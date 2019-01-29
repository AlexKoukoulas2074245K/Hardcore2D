//
//  PhysicsSystem.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/01/2019.
//

#include "PhysicsSystem.h"
#include "../util/MathUtils.h"
#include "../util/Logging.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/TransformComponent.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

static const std::unordered_map<PhysicsComponent::BodyType, std::vector<PhysicsComponent::BodyType>> sCollisionLayers =
{
    {PhysicsComponent::BodyType::DYNAMIC, {PhysicsComponent::BodyType::KINEMATIC, PhysicsComponent::BodyType::STATIC}},
    {PhysicsComponent::BodyType::KINEMATIC, {PhysicsComponent::BodyType::KINEMATIC, PhysicsComponent::BodyType::STATIC}},
    {PhysicsComponent::BodyType::STATIC, {}}
};

PhysicsSystem::PhysicsSystem(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
    
}

void PhysicsSystem::Initialize()
{
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
}

void PhysicsSystem::UpdateEntities(const std::vector<EntityNameIdEntry>& activeEntities, const float dt)
{
    for (const auto entityEntry: activeEntities)
    {
        const auto entityId = entityEntry.mEntityId;
        if (mEntityComponentManager->HasComponent<PhysicsComponent>(entityId) && 
            mEntityComponentManager->GetComponent<PhysicsComponent>(entityId).GetBodyType() != PhysicsComponent::BodyType::STATIC)
        {            
            auto& referenceEntityTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(entityId);
            auto& referenceEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(entityId);
            
            // Update velocity
            referenceEntityPhysicsComponent.GetVelocity() += referenceEntityPhysicsComponent.GetGravity() * dt;
            
            // Clamp velocity to min/maxes
            referenceEntityPhysicsComponent.GetVelocity() = ClampToMax(referenceEntityPhysicsComponent.GetVelocity(), referenceEntityPhysicsComponent.GetMaxVelocity());
            referenceEntityPhysicsComponent.GetVelocity() = ClampToMin(referenceEntityPhysicsComponent.GetVelocity(), referenceEntityPhysicsComponent.GetMinVelocity());
            
            // Update horizontal position first
            referenceEntityTransformComponent.GetTranslation().x += referenceEntityPhysicsComponent.GetVelocity().x * dt;
            
            // Find any entity collided with
            auto otherEntityId = CheckAndGetCollidedEntity(entityId, activeEntities);

            // If any were found, push the current entity outside of them
            if (otherEntityId != entityId)
            {
                PushEntityOutsideOtherEntityInAxis(entityId, otherEntityId, Axis::X_AXIS, dt);
            }
            
            // Update vertical position next
            referenceEntityTransformComponent.GetTranslation().y += referenceEntityPhysicsComponent.GetVelocity().y * dt;
            
            // Find any entity collided with
            otherEntityId = CheckAndGetCollidedEntity(entityId, activeEntities);
            
            // If any were found, push the current entity outside of them
            if (otherEntityId != entityId)
            {
                const auto& otherEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(otherEntityId);
                
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
        }
    }
    
    for (const auto entityEntry: activeEntities)
    {
        const auto entityId = entityEntry.mEntityId;
        if (mEntityComponentManager->HasComponent<TransformComponent>(entityId))
        {
            auto& referenceEntityTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(entityId);
            referenceEntityTransformComponent.GetPreviousTranslation() = referenceEntityTransformComponent.GetTranslation();
        }
    }
}

EntityId PhysicsSystem::CheckAndGetCollidedEntity(const EntityId referenceEntityId, const std::vector<EntityNameIdEntry>& allConsideredEntityIds)
{
    const auto& referenceEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(referenceEntityId);
    const auto& referenceEntityTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(referenceEntityId);
    const auto& referenceEntityHitBox = referenceEntityPhysicsComponent.GetHitBox();
    
    for (const auto otherEntityEntry: allConsideredEntityIds)
    {
        const auto otherEntityId = otherEntityEntry.mEntityId;
        const auto& collidableBodyTypesWithCurrent = sCollisionLayers.at(referenceEntityPhysicsComponent.GetBodyType());
        if (referenceEntityId != otherEntityId &&
            mEntityComponentManager->HasComponent<PhysicsComponent>(otherEntityId))
        {
            // Make sure the reference entity has this entity's body type in the collision layers
            const auto& otherEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(otherEntityId);
            if (std::find(collidableBodyTypesWithCurrent.cbegin(), collidableBodyTypesWithCurrent.cend(), otherEntityPhysicsComponent.GetBodyType()) == collidableBodyTypesWithCurrent.end())
            {
                continue;
            }
            
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
                return otherEntityId;
            }
        }
    }
    
    return referenceEntityId;
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
            const auto horizontalPushDelta = otherEntityTransformComponent.GetTranslation().x -
                                             otherEntityHitBox.mCenterPoint.x -
                                             otherEntityHitBox.mDimensions.x * 0.5f -
                                             referenceEntityHitBox.mCenterPoint.x -
                                             referenceEntityHitBox.mDimensions.x * 0.5f;
            
            if (otherEntityPhysicsComponent.GetBodyType() != PhysicsComponent::BodyType::KINEMATIC ||
                Abs(-referenceEntityTransformComponent.GetTranslation().x + horizontalPushDelta) < 10.0f)
            {
                referenceEntityTransformComponent.GetTranslation().x = horizontalPushDelta;
            }
        }
        // Collided with entity to the left
        else
        {
            const auto horizontalPushDelta = otherEntityTransformComponent.GetTranslation().x -
                                             otherEntityHitBox.mCenterPoint.x +
                                             otherEntityHitBox.mDimensions.x * 0.5f -
                                             referenceEntityHitBox.mCenterPoint.x +
                                             referenceEntityHitBox.mDimensions.x * 0.5f;
            
            if (otherEntityPhysicsComponent.GetBodyType() != PhysicsComponent::BodyType::KINEMATIC ||
                Abs(-referenceEntityTransformComponent.GetTranslation().x + horizontalPushDelta) < 10.0f)
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
                referenceEntityTransformComponent.GetTranslation().y = otherEntityTransformComponent.GetTranslation().y -
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
                referenceEntityTransformComponent.GetTranslation().y = otherEntityTransformComponent.GetTranslation().y -
                                                                       otherEntityHitBox.mCenterPoint.y +
                                                                       otherEntityHitBox.mDimensions.y * 0.5f -
                                                                       referenceEntityHitBox.mCenterPoint.y +
                                                                       referenceEntityHitBox.mDimensions.y * 0.5f;
            }
        }
    }
    
}
