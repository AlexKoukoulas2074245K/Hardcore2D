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


void PhysicsSystem::UpdateEntities(const std::vector<EntityId>& entityIds, const float dt)
{
    auto& entityComponentManager = mServiceLocator.ResolveService<EntityComponentManager>();
    for (const auto entityId: entityIds)
    {
        if (entityComponentManager.HasComponent<PhysicsComponent>(entityId))
        {
            
            auto& transformationComponent = entityComponentManager.GetComponent<TransformationComponent>(entityId);
            auto& physicsComponent = entityComponentManager.GetComponent<PhysicsComponent>(entityId);
            
            physicsComponent.GetVelocity() = ClampToMax(physicsComponent.GetVelocity(), physicsComponent.GetMaxVelocity());
            physicsComponent.GetVelocity() = ClampToMin(physicsComponent.GetVelocity(), physicsComponent.GetMinVelocity());
            
            transformationComponent.GetTranslation() += physicsComponent.GetVelocity() * dt + 0.5f * physicsComponent.GetGravity() * dt * dt;
            physicsComponent.GetVelocity() += physicsComponent.GetGravity() * dt;
            
            if (physicsComponent.GetBodyType() == PhysicsComponent::BodyType::DYNAMIC)
            {
                if (transformationComponent.GetTranslation().y < 160.0f)
                {
                    transformationComponent.GetTranslation().y = 160.0f;
                    physicsComponent.GetVelocity() = glm::vec3(0.0f, 0.0f, 0.0f);
                }
            }
        }
    }
}
