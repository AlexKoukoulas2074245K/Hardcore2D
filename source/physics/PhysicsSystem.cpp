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
static unsigned char COLLISION_DIRECTION_WEST = 0x1;
static unsigned char COLLISION_DIRECTION_NORTH = 0x2;
static unsigned char COLLISION_DIRECTION_EAST = 0x4;
static unsigned char COLLISION_DIRECTION_SOUTH = 0x8;

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
            
            const auto collisionInfo = CheckCollisions(entityId, entityIds);
            
            if (collisionInfo.first != entityId)
            {
                const auto& thisHitBox = physicsComponent.GetHitBox();
                const auto& otherEntityHitBox = mEntityComponentManager->GetComponent<PhysicsComponent>(collisionInfo.first).GetHitBox();
                const auto& otherEntityTransf = mEntityComponentManager->GetComponent<TransformationComponent>(collisionInfo.first);
            }

            if (collisionInfo.first != entityId)
            {
                const auto& thisHitBox = physicsComponent.GetHitBox();
                const auto& otherEntityHitBox = mEntityComponentManager->GetComponent<PhysicsComponent>(collisionInfo.first).GetHitBox();
                const auto& otherEntityTransf = mEntityComponentManager->GetComponent<TransformationComponent>(collisionInfo.first);                
                if (collisionInfo.second & COLLISION_DIRECTION_WEST)
                {
                    //if ((transformationComponent.GetTranslation().y < (otherEntityTransf.GetTranslation().y + thisHitBox.mDimensions.y * 0.5f)) &&
                    //    (transformationComponent.GetTranslation().y > (otherEntityTransf.GetTranslation().y - thisHitBox.mDimensions.y * 0.5f)))
                    //{
                        
                    //}                                        
                }
                if (collisionInfo.second & COLLISION_DIRECTION_EAST)
                {
                    //if ((transformationComponent.GetTranslation().y < (otherEntityTransf.GetTranslation().y + thisHitBox.mDimensions.y * 0.5f)) &&
                    //    (transformationComponent.GetTranslation().y >(otherEntityTransf.GetTranslation().y - thisHitBox.mDimensions.y * 0.5f)))
                    //{
                        transformationComponent.GetTranslation().x = otherEntityTransf.GetTranslation().x - otherEntityHitBox.mDimensions.x * 0.5f - thisHitBox.mDimensions.x * 0.5f;
                    //}
                }
                if (collisionInfo.second & COLLISION_DIRECTION_NORTH)
                {
                    //if (!((transformationComponent.GetTranslation().x > (otherEntityTransf.GetTranslation().x + thisHitBox.mDimensions.x * 0.5f)) ||
                      //  (transformationComponent.GetTranslation().x < (otherEntityTransf.GetTranslation().x - thisHitBox.mDimensions.x * 0.5f))))
                    //{
                        transformationComponent.GetTranslation().y = otherEntityTransf.GetTranslation().y - otherEntityHitBox.mDimensions.y * 0.5f - thisHitBox.mDimensions.y * 0.5f;
                    //}
                }
                if (collisionInfo.second & COLLISION_DIRECTION_SOUTH)
                {
                    //if (!((transformationComponent.GetTranslation().x >(otherEntityTransf.GetTranslation().x + thisHitBox.mDimensions.x * 0.5f)) ||
                     //   (transformationComponent.GetTranslation().x < (otherEntityTransf.GetTranslation().x - thisHitBox.mDimensions.x * 0.5f))))
                    //{
                        transformationComponent.GetTranslation().y = otherEntityTransf.GetTranslation().y + otherEntityHitBox.mDimensions.y * 0.5f + thisHitBox.mDimensions.y * 0.5f;
                    //}
                }
            }            
        }
    }
}

PhysicsSystem::CollisionInfo PhysicsSystem::CheckCollisions(const EntityId referenceId, const std::vector<EntityId>& entityIds)
{
    CollisionInfo collisionInfo = std::make_pair(referenceId, NO_COLLISION);
    
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
                // Intersection on the xAxis
                if (xAxisTest > yAxisTest)
                {
                    Log(LogType::INFO, "%.2f, %.2f", xAxisTest, yAxisTest);
                    collisionInfo.second |= (rectBX - rectAX > 0.0f ? COLLISION_DIRECTION_EAST : COLLISION_DIRECTION_WEST);
                }
                // Intersection on the yAxis
                else
                {
                    Log(LogType::INFO, "%.2f, %.2f", xAxisTest, yAxisTest);
                    collisionInfo.second |= (rectBY - rectAY > 0.0f ? COLLISION_DIRECTION_NORTH : COLLISION_DIRECTION_SOUTH);
                }
                
                collisionInfo.first = otherEntityId;
            }
        }
    }
    
    return collisionInfo;
}
