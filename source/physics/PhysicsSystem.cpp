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
#include "../components/TransformationComponent.h"

PhysicsSystem::PhysicsSystem(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
    
}

void PhysicsSystem::Initialize()
{
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
}

void PhysicsSystem::UpdateEntities(const std::vector<EntityId>& entityIds, const float dt)
{
    for (const auto entityId: entityIds)
    {
        if (mEntityComponentManager->HasComponent<PhysicsComponent>(entityId) && 
            mEntityComponentManager->GetComponent<PhysicsComponent>(entityId).GetBodyType() != PhysicsComponent::BodyType::STATIC)
        {            
            auto& thisEntityTransformationComponent = mEntityComponentManager->GetComponent<TransformationComponent>(entityId);
            auto& thisEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(entityId);
            
            // Update velocity
            thisEntityPhysicsComponent.GetVelocity() += thisEntityPhysicsComponent.GetGravity() * dt;
            
            // Clamp velocity to min/maxes
            thisEntityPhysicsComponent.GetVelocity() = ClampToMax(thisEntityPhysicsComponent.GetVelocity(), thisEntityPhysicsComponent.GetMaxVelocity());
            thisEntityPhysicsComponent.GetVelocity() = ClampToMin(thisEntityPhysicsComponent.GetVelocity(), thisEntityPhysicsComponent.GetMinVelocity());
            
            // Update horizontal position first
            thisEntityTransformationComponent.GetTranslation().x += thisEntityPhysicsComponent.GetVelocity().x * dt;
            
            // Find any entity collided with
            auto collisionCheckEntityId = CheckAndGetCollidedEntity(entityId, entityIds);            

            // If any were found, push the current entity outside of them
            if (collisionCheckEntityId != entityId &&                 
                mEntityComponentManager->GetComponent<PhysicsComponent>(collisionCheckEntityId).GetBodyType() != PhysicsComponent::BodyType::DYNAMIC)
            {
                PushEntityOutsideOtherEntityInAxis(entityId, collisionCheckEntityId, Axis::X_AXIS);
            }
            
            // Update vertical position next
            thisEntityTransformationComponent.GetTranslation().y += thisEntityPhysicsComponent.GetVelocity().y * dt;
            
            // Find any entity collided with
            collisionCheckEntityId = CheckAndGetCollidedEntity(entityId, entityIds);            

            // If any were found, push the current entity outside of them
            if (collisionCheckEntityId != entityId &&
                mEntityComponentManager->GetComponent<PhysicsComponent>(collisionCheckEntityId).GetBodyType() != PhysicsComponent::BodyType::DYNAMIC)
            {
                PushEntityOutsideOtherEntityInAxis(entityId, collisionCheckEntityId, Axis::Y_AXIS);
            }
        }
    }
}

EntityId PhysicsSystem::CheckAndGetCollidedEntity(const EntityId referenceEntityId, const std::vector<EntityId>& allConsideredEntityIds)
{
    for (const EntityId otherEntityId: allConsideredEntityIds)
    {
        if (referenceEntityId != otherEntityId && mEntityComponentManager->HasComponent<PhysicsComponent>(otherEntityId))
        {
            auto& transfA = mEntityComponentManager->GetComponent<TransformationComponent>(referenceEntityId);
            auto& transfB = mEntityComponentManager->GetComponent<TransformationComponent>(otherEntityId);
            
            const auto& hitBoxA = mEntityComponentManager->GetComponent<PhysicsComponent>(referenceEntityId).GetHitBox();
            const auto& hitBoxB = mEntityComponentManager->GetComponent<PhysicsComponent>(otherEntityId).GetHitBox();

            const auto rectAX = transfA.GetTranslation().x + hitBoxA.mCenterPoint.x;
            const auto rectAY = transfA.GetTranslation().y + hitBoxA.mCenterPoint.y;
            
            const auto rectBX = transfB.GetTranslation().x + hitBoxB.mCenterPoint.x;
            const auto rectBY = transfB.GetTranslation().y + hitBoxB.mCenterPoint.y;

            const auto xAxisTest = Abs(rectAX - rectBX) * 2.0f - (hitBoxA.mDimensions.x + hitBoxB.mDimensions.x);
            const auto yAxisTest = Abs(rectAY - rectBY) * 2.0f - (hitBoxA.mDimensions.y + hitBoxB.mDimensions.y);
            
            if (xAxisTest < 0 && yAxisTest < 0)
            {
                return otherEntityId;
            }
        }
    }
    
    return referenceEntityId;
}

void PhysicsSystem::PushEntityOutsideOtherEntityInAxis(const EntityId referenceEntityId, const EntityId collidedWithEntityId, const PhysicsSystem::Axis axis)
{
    auto& thisEntityTransformationComponent = mEntityComponentManager->GetComponent<TransformationComponent>(referenceEntityId);
    auto& thisEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(referenceEntityId);

    const auto& thisEntityHitBox = thisEntityPhysicsComponent.GetHitBox();
    const auto& otherEntityHitBox = mEntityComponentManager->GetComponent<PhysicsComponent>(collidedWithEntityId).GetHitBox();
    const auto& otherEntityTransf = mEntityComponentManager->GetComponent<TransformationComponent>(collidedWithEntityId);

    if (axis == Axis::X_AXIS)
    {
        // Collided with entity to the right
        if (thisEntityTransformationComponent.GetTranslation().x < otherEntityTransf.GetTranslation().x)
        {
            thisEntityTransformationComponent.GetTranslation().x = otherEntityTransf.GetTranslation().x - otherEntityHitBox.mDimensions.x * 0.5f - thisEntityHitBox.mDimensions.x * 0.5f;
        }
        // Collided with entity to the left
        else
        {
            thisEntityTransformationComponent.GetTranslation().x = otherEntityTransf.GetTranslation().x + otherEntityHitBox.mDimensions.x * 0.5f + thisEntityHitBox.mDimensions.x * 0.5f;
        }
    }
    else
    {
        // Collided with entity above
        if (thisEntityTransformationComponent.GetTranslation().y < otherEntityTransf.GetTranslation().y)
        {
            thisEntityTransformationComponent.GetTranslation().y = otherEntityTransf.GetTranslation().y - otherEntityHitBox.mDimensions.y * 0.5f - thisEntityHitBox.mDimensions.y * 0.5f;
            thisEntityPhysicsComponent.GetVelocity().y = 0.0f;
        }
        // Collided with entity below
        else
        {
            thisEntityTransformationComponent.GetTranslation().y = otherEntityTransf.GetTranslation().y + otherEntityHitBox.mDimensions.y * 0.5f + thisEntityHitBox.mDimensions.y * 0.5f;
        }
    }
    
}
