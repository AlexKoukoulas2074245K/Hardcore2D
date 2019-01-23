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
            
            auto& transformationComponent = mEntityComponentManager->GetComponent<TransformationComponent>(entityId);
            auto& physicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(entityId);
            
            physicsComponent.GetVelocity() += physicsComponent.GetGravity() * dt;
            
            physicsComponent.GetVelocity() = ClampToMax(physicsComponent.GetVelocity(), physicsComponent.GetMaxVelocity());
            physicsComponent.GetVelocity() = ClampToMin(physicsComponent.GetVelocity(), physicsComponent.GetMinVelocity());
            
            transformationComponent.GetTranslation().x += physicsComponent.GetVelocity().x * dt;
            
            auto collisionCheckEntityId = CheckCollisions(entityId, entityIds);
            
            if (collisionCheckEntityId != entityId)
            {
                const auto& thisHitBox = physicsComponent.GetHitBox();
                const auto& otherEntityHitBox = mEntityComponentManager->GetComponent<PhysicsComponent>(collisionCheckEntityId).GetHitBox();
                const auto& otherEntityTransf = mEntityComponentManager->GetComponent<TransformationComponent>(collisionCheckEntityId);
                
                if (transformationComponent.GetTranslation().x < otherEntityTransf.GetTranslation().x)
                {
                    transformationComponent.GetTranslation().x = otherEntityTransf.GetTranslation().x - otherEntityHitBox.mDimensions.x * 0.5f - thisHitBox.mDimensions.x * 0.5f;
                }
                else
                {
                    transformationComponent.GetTranslation().x = otherEntityTransf.GetTranslation().x + otherEntityHitBox.mDimensions.x * 0.5f + thisHitBox.mDimensions.x * 0.5f;
                }
            }
            
            transformationComponent.GetTranslation().y += physicsComponent.GetVelocity().y * dt;
            
            collisionCheckEntityId = CheckCollisions(entityId, entityIds);
            
            if (collisionCheckEntityId != entityId)
            {
                const auto& thisHitBox = physicsComponent.GetHitBox();
                const auto& otherEntityHitBox = mEntityComponentManager->GetComponent<PhysicsComponent>(collisionCheckEntityId).GetHitBox();
                const auto& otherEntityTransf = mEntityComponentManager->GetComponent<TransformationComponent>(collisionCheckEntityId);
                
                if (transformationComponent.GetTranslation().y < otherEntityTransf.GetTranslation().y)
                {
                    transformationComponent.GetTranslation().y = otherEntityTransf.GetTranslation().y - otherEntityHitBox.mDimensions.y * 0.5f - thisHitBox.mDimensions.y * 0.5f;
                    physicsComponent.GetVelocity().y = 0.0f;
                }
                else
                {
                    transformationComponent.GetTranslation().y = otherEntityTransf.GetTranslation().y + otherEntityHitBox.mDimensions.y * 0.5f + thisHitBox.mDimensions.y * 0.5f;
                }
            }
        }
    }
}

EntityId PhysicsSystem::CheckCollisions(const EntityId referenceId, const std::vector<EntityId>& entityIds)
{
    for (const EntityId otherEntityId: entityIds)
    {
        if (referenceId != otherEntityId && mEntityComponentManager->HasComponent<PhysicsComponent>(otherEntityId))
        {
            auto& transfA = mEntityComponentManager->GetComponent<TransformationComponent>(referenceId);
            auto& transfB = mEntityComponentManager->GetComponent<TransformationComponent>(otherEntityId);
            
            const auto& hitBoxA = mEntityComponentManager->GetComponent<PhysicsComponent>(referenceId).GetHitBox();
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
    
    return referenceId;
}
