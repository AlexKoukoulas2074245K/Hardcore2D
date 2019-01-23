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

static unsigned char NO_COLLISION = 0x0;
static unsigned char COLLISION_DIRECTION_HORIZONTAL = 0x1;
static unsigned char COLLISION_DIRECTION_VERTICAL = 0x2;

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
            
            transformationComponent.GetTranslation() += physicsComponent.GetVelocity() * dt;
            
            const auto collisionResult = CheckCollisions(entityId, entityIds);
            
            if (collisionResult & COLLISION_DIRECTION_HORIZONTAL)
            {
                transformationComponent.GetTranslation().x -= physicsComponent.GetVelocity().x * dt;
            }
            if (collisionResult & COLLISION_DIRECTION_VERTICAL)
            {
                transformationComponent.GetTranslation().y -= physicsComponent.GetVelocity().y * dt;
            }
        }
    }
}

unsigned char PhysicsSystem::CheckCollisions(const EntityId referenceId, const std::vector<EntityId>& entityIds)
{
    unsigned char collisionResult = NO_COLLISION;
    
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
                if (xAxisTest < -20.0f || yAxisTest < -20.0f)
                {
                    if (xAxisTest > yAxisTest)
                    {
                        Log(LogType::INFO, "%.2f, %.2f", xAxisTest, yAxisTest);
                    }
                    collisionResult |= (xAxisTest > yAxisTest ? COLLISION_DIRECTION_HORIZONTAL : COLLISION_DIRECTION_VERTICAL);
                }
            }
        }
    }
    
    return collisionResult;
}
