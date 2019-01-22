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
            
            transformationComponent.GetTranslation() += physicsComponent.GetVelocity() * dt;
            
            if (CheckForCollisions(entityId, entityIds))
            {
                transformationComponent.GetTranslation() -= physicsComponent.GetVelocity() * dt;
            }            
        }
    }
}

bool PhysicsSystem::CheckForCollisions(const EntityId referenceId, const std::vector<EntityId>& entityIds)
{
    for (const EntityId otherEntityId: entityIds)
    {
        if (referenceId != otherEntityId)
        {
            const auto& transfA = mEntityComponentManager->GetComponent<TransformationComponent>(referenceId);
            const auto& transfB = mEntityComponentManager->GetComponent<TransformationComponent>(otherEntityId);
            
            //auto& physicsA = mEntityComponentManager->GetComponent<PhysicsComponent>(referenceId);
            //const auto& physicsB = mEntityComponentManager->GetComponent<PhysicsComponent>(otherEntityId);                        

            const auto rectAX = transfA.GetTranslation().x;
            const auto rectAY = transfA.GetTranslation().y;
            
            const auto rectBX = transfB.GetTranslation().x;
            const auto rectBY = transfB.GetTranslation().y;

            const auto rectAWidth = transfA.GetScale().x;
            const auto rectBWidth = transfB.GetScale().x;

            const auto rectAHeight = transfA.GetScale().y;
            const auto rectBHeight = transfB.GetScale().y;

            if (Abs(rectAX - rectBX) * 2.0f < (rectAWidth + rectBWidth) &&
                Abs(rectAY - rectBY) * 2.0f < (rectAHeight + rectBHeight))
            {        
                Log(LogType::INFO, "Deltas in: %.2f, %.2f", ((rectAWidth + rectBWidth) - Abs(rectAX - rectBX) * 2.0f), ((rectAHeight + rectBHeight) - Abs(rectAY - rectBY) * 2.0f));
                return true;
            }
        }
    }
    
    return false;
}
