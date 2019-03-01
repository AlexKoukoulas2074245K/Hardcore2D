//
//  PhysicsSystem.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/01/2019.
//

#include "PhysicsSystem.h"
#include "../scene_graphs/QuadtreeSceneGraph.h"
#include "../util/MathUtils.h"
#include "../util/Logging.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/TransformComponent.h"
#include "../events/EventCommunicator.h"
#include "../events/EntityCollisionEvent.h"
#include "../events/LevelCreatedEvent.h"

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>
#include <SDL.h>

static const std::unordered_map<PhysicsComponent::BodyType, std::vector<PhysicsComponent::BodyType>> sPhysicallyInteractableLayers =
{
    {PhysicsComponent::BodyType::DYNAMIC, {PhysicsComponent::BodyType::KINEMATIC, PhysicsComponent::BodyType::STATIC}},
    {PhysicsComponent::BodyType::KINEMATIC, {PhysicsComponent::BodyType::KINEMATIC, PhysicsComponent::BodyType::STATIC}},
    {PhysicsComponent::BodyType::STATIC, {}}
};

PhysicsSystem::PhysicsSystem(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
    
}

PhysicsSystem::~PhysicsSystem()
{
}

bool PhysicsSystem::VInitialize()
{
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
    mEventCommunicator = mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    RegisterEventCallbacks();
    return true;
}

void PhysicsSystem::UpdateEntities(const std::vector<EntityNameIdEntry>& activeEntities, const float dt)
{
    mSceneGraph->VClear();
    mSceneGraph->VPopulateSceneGraph(activeEntities);
    
    for (const auto entityEntry: activeEntities)
    {
        const auto entityId = entityEntry.mEntityId;
        if (mEntityComponentManager->HasComponent<PhysicsComponent>(entityId) && 
            mEntityComponentManager->GetComponent<PhysicsComponent>(entityId).GetBodyType() != PhysicsComponent::BodyType::STATIC)
        {            
            auto& referenceEntityTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(entityId);
            auto& referenceEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(entityId);
            
            // Update angular velocity
            referenceEntityTransformComponent.GetRotation().z += referenceEntityPhysicsComponent.GetAngularVelocity();

            // Update velocity            
            referenceEntityPhysicsComponent.GetVelocity() += referenceEntityPhysicsComponent.GetGravity() * dt;
            
            // Clamp velocity to min/maxes
            referenceEntityPhysicsComponent.GetVelocity() = ClampToMax(referenceEntityPhysicsComponent.GetVelocity(), referenceEntityPhysicsComponent.GetMaxVelocity());
            referenceEntityPhysicsComponent.GetVelocity() = ClampToMin(referenceEntityPhysicsComponent.GetVelocity(), referenceEntityPhysicsComponent.GetMinVelocity());
            
            // Update horizontal position first
            if (mEntityComponentManager->HasEntityEntry(referenceEntityTransformComponent.GetParent()))
            {
                const auto& parentTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(referenceEntityTransformComponent.GetParent());
                referenceEntityTransformComponent.GetRelativeTranslationToParent().x += referenceEntityPhysicsComponent.GetVelocity().x * dt;
                referenceEntityTransformComponent.GetTranslation().x = parentTransformComponent.GetTranslation().x + referenceEntityTransformComponent.GetRelativeTranslationToParent().x;
            }
            else
            {
                referenceEntityTransformComponent.GetTranslation().x += referenceEntityPhysicsComponent.GetVelocity().x * dt;
            }
            
            
            // Set of all collidedEntities (filtered by layers and not)
            std::set<EntityId> allCollidedEntities;
            std::string collisionInfo;
            
            // Find all entities horizontally colliding with current
            auto horizontallyCollidingEntityIds = CheckAndGetCollidedEntities(entityId, mSceneGraph->VGetCollisionCandidates(entityId));
            
            const auto& collidableBodyTypesWithCurrent = sPhysicallyInteractableLayers.at(referenceEntityPhysicsComponent.GetBodyType());
            for (const auto otherEntityId: horizontallyCollidingEntityIds)
            {
                allCollidedEntities.insert(otherEntityId);
                
                // Make sure the reference entity has this entity's body type in the collision layers
                const auto& otherEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(otherEntityId);
                if (std::find(collidableBodyTypesWithCurrent.cbegin(), collidableBodyTypesWithCurrent.cend(), otherEntityPhysicsComponent.GetBodyType()) == collidableBodyTypesWithCurrent.end())
                {
                    continue;
                }
                
                if (mEntityComponentManager->HasEntityEntry(referenceEntityTransformComponent.GetParent()))
                {
                    continue;
                }
                
                // Push reference entity outside of other horizontally colliding entity
                PushEntityOutsideOtherEntityInAxis(entityId, otherEntityId, Axis::X_AXIS, dt);
            }
            
            // Update vertical position next
            if (mEntityComponentManager->HasEntityEntry(referenceEntityTransformComponent.GetParent()))
            {
                const auto& parentTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(referenceEntityTransformComponent.GetParent());
                referenceEntityTransformComponent.GetRelativeTranslationToParent().y += referenceEntityPhysicsComponent.GetVelocity().y * dt;
                referenceEntityTransformComponent.GetTranslation().y = parentTransformComponent.GetTranslation().y + referenceEntityTransformComponent.GetRelativeTranslationToParent().y;
            }
            else
            {
                referenceEntityTransformComponent.GetTranslation().y += referenceEntityPhysicsComponent.GetVelocity().y * dt;
            }

            // Find all entities vertically colliding with current
            auto verticallyCollidingEntityIds = CheckAndGetCollidedEntities(entityId, mSceneGraph->VGetCollisionCandidates(entityId));
            
            for (const auto otherEntityId: verticallyCollidingEntityIds)
            {
                allCollidedEntities.insert(otherEntityId);
                
                const auto& otherEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(otherEntityId);
                if (std::find(collidableBodyTypesWithCurrent.cbegin(), collidableBodyTypesWithCurrent.cend(), otherEntityPhysicsComponent.GetBodyType()) == collidableBodyTypesWithCurrent.end())
                {
                    continue;
                }
                
                if (mEntityComponentManager->HasEntityEntry(referenceEntityTransformComponent.GetParent()))
                {
                    continue;
                }
                
                // Push reference entity outside of other vertically colliding entity
                PushEntityOutsideOtherEntityInAxis(entityId, otherEntityId, Axis::Y_AXIS, dt);
                    
                // In the case of a Kinematic object append its deltas to current entity
                if (otherEntityPhysicsComponent.GetBodyType() == PhysicsComponent::BodyType::KINEMATIC)
                {
                    const auto& otherEntityTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(otherEntityId);
                    
                    // Before adding the kinematic object's horizontal velocity, make sure that the kinematic object itself
                    // is not horizontally blocked by another object
                    if (Abs(otherEntityTransformComponent.GetPreviousTranslation().x - otherEntityTransformComponent.GetTranslation().x) > 0.01f)
                    {
                        referenceEntityTransformComponent.GetTranslation().x += otherEntityPhysicsComponent.GetVelocity().x * dt;
                    }
                    
                    // Before adding the kinematic object's vertical velocity, make sure that the kinematic object itself
                    // is not vertically blocked by another object
                    if (Abs(otherEntityTransformComponent.GetPreviousTranslation().y - otherEntityTransformComponent.GetTranslation().y) > 0.01f)
                    {
                        referenceEntityTransformComponent.GetTranslation().y += otherEntityPhysicsComponent.GetVelocity().y * dt;
                    }
                }
            }
            
            // Generate collision events for all collided entities (physically interactable and not)
            for (const auto otherEntityId: allCollidedEntities)
            {
                mEventCommunicator->DispatchEvent(std::make_unique<EntityCollisionEvent>(std::make_pair(entityId, otherEntityId)));

                // If the second collided entity is static, we need to manually dispatch a collision event with it as the first entity id, since
                // the logic above will not run for static objects, and we need to account for them as first collided entries for damage events etc
                if (mEntityComponentManager->GetComponent<PhysicsComponent>(otherEntityId).GetBodyType() == PhysicsComponent::BodyType::STATIC)
                {
                    mEventCommunicator->DispatchEvent(std::make_unique<EntityCollisionEvent>(std::make_pair(otherEntityId, entityId)));
                }
            }
            
        }
    }

    for (const auto entityEntry : activeEntities)
    {
        const auto entityId = entityEntry.mEntityId;
        if (mEntityComponentManager->HasComponent<TransformComponent>(entityId))
        {
            auto& transformComponent = mEntityComponentManager->GetComponent<TransformComponent>(entityId);
            transformComponent.GetPreviousTranslation() = transformComponent.GetTranslation();
        }
    }
}

void PhysicsSystem::RegisterEventCallbacks()
{
    mEventCommunicator->RegisterEventCallback<LevelCreatedEvent>([this](const IEvent& event)
    {
        const auto& actualEvent = static_cast<const LevelCreatedEvent&>(event);
        mSceneGraph = std::make_unique<QuadtreeSceneGraph>(*mEntityComponentManager, glm::vec2(0.0f, 0.0f), glm::vec2(actualEvent.GetLevelWidth(), actualEvent.GetLevelHeight()));
    });
}

std::list<EntityId> PhysicsSystem::CheckAndGetCollidedEntities(const EntityId referenceEntityId, const std::list<EntityId>& allConsideredEntityIds)
{
    std::list<EntityId> collidedEntities;
    
    const auto& referenceEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(referenceEntityId);
    const auto& referenceEntityTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(referenceEntityId);
    const auto& referenceEntityHitBox = referenceEntityPhysicsComponent.GetHitBox();
    
    for (const auto otherEntityId: allConsideredEntityIds)
    {                
        if (referenceEntityId != otherEntityId &&
            mEntityComponentManager->HasComponent<PhysicsComponent>(otherEntityId))
        {
            const auto& otherEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(otherEntityId);
            const auto& otherEntityTransformComponentormComponent = mEntityComponentManager->GetComponent<TransformComponent>(otherEntityId);
            const auto& otherEntityHitBox = otherEntityPhysicsComponent.GetHitBox();

            const auto rectAX = referenceEntityTransformComponent.GetTranslation().x + referenceEntityHitBox.mCenterPoint.x;
            const auto rectAY = referenceEntityTransformComponent.GetTranslation().y + referenceEntityHitBox.mCenterPoint.y;
            
            const auto rectBX = otherEntityTransformComponentormComponent.GetTranslation().x + otherEntityHitBox.mCenterPoint.x;
            const auto rectBY = otherEntityTransformComponentormComponent.GetTranslation().y + otherEntityHitBox.mCenterPoint.y;

            const auto xAxisTest = Abs(rectAX - rectBX) * 2.0f - (referenceEntityHitBox.mDimensions.x + otherEntityHitBox.mDimensions.x);
            const auto yAxisTest = Abs(rectAY - rectBY) * 2.0f - (referenceEntityHitBox.mDimensions.y + otherEntityHitBox.mDimensions.y);
            
            if (xAxisTest < 0 && yAxisTest < 0)
            {
                collidedEntities.push_back(otherEntityId);
            }
        }
    }
    
    return collidedEntities;
}

void PhysicsSystem::PushEntityOutsideOtherEntityInAxis(const EntityId referenceEntityId, const EntityId collidedWithEntityId, const PhysicsSystem::Axis axis, const float dt)
{
    auto& referenceEntityTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(referenceEntityId);
    auto& referenceEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(referenceEntityId);

    const auto& referenceEntityHitBox = referenceEntityPhysicsComponent.GetHitBox();
    const auto& otherEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(collidedWithEntityId);
    const auto& otherEntityHitBox = otherEntityPhysicsComponent.GetHitBox();
    const auto& otherEntityTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(collidedWithEntityId);
    
    if (axis == Axis::X_AXIS)
    {
        // Collided with entity to the right
        if (referenceEntityTransformComponent.GetTranslation().x < otherEntityTransformComponent.GetTranslation().x)
        {
            const auto horizontalPushDelta = otherEntityTransformComponent.GetTranslation().x +
                                             otherEntityHitBox.mCenterPoint.x -
                                             otherEntityHitBox.mDimensions.x * 0.5f -
                                             referenceEntityHitBox.mCenterPoint.x -
                                             referenceEntityHitBox.mDimensions.x * 0.5f;
            
            if (Abs(referenceEntityTransformComponent.GetTranslation().x - (otherEntityTransformComponent.GetTranslation().x + otherEntityHitBox.mCenterPoint.x)) > otherEntityPhysicsComponent.GetHitBox().mDimensions.x * 0.5f)
            {
                referenceEntityTransformComponent.GetTranslation().x = horizontalPushDelta;
            }            
        }
        // Collided with entity to the left
        else
        {
            const auto horizontalPushDelta = otherEntityTransformComponent.GetTranslation().x +
                                             otherEntityHitBox.mCenterPoint.x +
                                             otherEntityHitBox.mDimensions.x * 0.5f -
                                             referenceEntityHitBox.mCenterPoint.x +
                                             referenceEntityHitBox.mDimensions.x * 0.5f;
            
            if (Abs(referenceEntityTransformComponent.GetTranslation().x - (otherEntityTransformComponent.GetTranslation().x + otherEntityHitBox.mCenterPoint.x)) > otherEntityPhysicsComponent.GetHitBox().mDimensions.x * 0.5f)
            {
                referenceEntityTransformComponent.GetTranslation().x = horizontalPushDelta;
            }
        }
    }
    else
    {
        // Collided with entity above
        if (referenceEntityTransformComponent.GetTranslation().y < otherEntityTransformComponent.GetTranslation().y)
        {
            if (otherEntityPhysicsComponent.GetBodyType() == PhysicsComponent::BodyType::KINEMATIC)
            {
                referenceEntityTransformComponent.GetTranslation().y -= referenceEntityPhysicsComponent.GetVelocity().y * dt;
                referenceEntityPhysicsComponent.GetVelocity().y = 0.0f;
            }
            else
            {
                referenceEntityTransformComponent.GetTranslation().y = otherEntityTransformComponent.GetTranslation().y +
                                                                       otherEntityHitBox.mCenterPoint.y -
                                                                       otherEntityHitBox.mDimensions.y * 0.5f -
                                                                       referenceEntityHitBox.mCenterPoint.y -
                                                                       referenceEntityHitBox.mDimensions.y * 0.5f;
                referenceEntityPhysicsComponent.GetVelocity().y = 0.0f;
            }
        }
        // Collided with entity below
        else
        {
            if (otherEntityPhysicsComponent.GetBodyType() == PhysicsComponent::BodyType::KINEMATIC)
            {
                referenceEntityTransformComponent.GetTranslation().y -= referenceEntityPhysicsComponent.GetVelocity().y * dt;                
            }
            else
            {
                referenceEntityTransformComponent.GetTranslation().y = otherEntityTransformComponent.GetTranslation().y +
                                                                       otherEntityHitBox.mCenterPoint.y +
                                                                       otherEntityHitBox.mDimensions.y * 0.5f -
                                                                       referenceEntityHitBox.mCenterPoint.y +
                                                                       referenceEntityHitBox.mDimensions.y * 0.5f;
                referenceEntityPhysicsComponent.GetVelocity().y = 0.0f;
            }
        }
    }
}
