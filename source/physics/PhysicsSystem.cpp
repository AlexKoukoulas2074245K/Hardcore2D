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
        if (mEntityComponentManager->HasComponent<PhysicsComponent>(entityId))
        {
            
            auto& transformationComponent = mEntityComponentManager->GetComponent<TransformationComponent>(entityId);
            auto& physicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(entityId);
            
            physicsComponent.GetVelocity() += physicsComponent.GetGravity() * dt;
            
            physicsComponent.GetVelocity() = ClampToMax(physicsComponent.GetVelocity(), physicsComponent.GetMaxVelocity());
            physicsComponent.GetVelocity() = ClampToMin(physicsComponent.GetVelocity(), physicsComponent.GetMinVelocity());
            
            transformationComponent.GetTranslation() += physicsComponent.GetVelocity() * dt;
            
            CheckForCollisions(entityId, entityIds);
        }
    }
}

void PhysicsSystem::CheckForCollisions(const EntityId referenceId, const std::vector<EntityId>& entityIds)
{
    for (const EntityId otherEntityId: entityIds)
    {
        if (referenceId != otherEntityId)
        {
            const auto& transfA = mEntityComponentManager->GetComponent<TransformationComponent>(referenceId);
            const auto& transfB = mEntityComponentManager->GetComponent<TransformationComponent>(otherEntityId);
            
            auto& physicsA = mEntityComponentManager->GetComponent<PhysicsComponent>(referenceId);
            //const auto& physicsB = mEntityComponentManager->GetComponent<PhysicsComponent>(otherEntityId);
            
            if (transfA.GetTranslation().x + transfA.GetScale().x > transfB.GetTranslation().x &&
                transfA.GetTranslation().y + transfA.GetScale().y > transfB.GetTranslation().y &&
                transfB.GetTranslation().x + transfB.GetScale().x > transfA.GetTranslation().x &&
                transfB.GetTranslation().y + transfB.GetScale().y > transfB.GetTranslation().y)
            {
                physicsA.GetVelocity() = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        }
    }
}
