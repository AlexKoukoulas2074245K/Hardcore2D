//
//  VerticalMovingPlatformAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 23/02/2019.
//

const float VerticalMovingPlatformAIComponent::MAX_DISTANCE_FROM_INIT_POSITION = 600.0f;

#include "VerticalMovingPlatformAIComponent.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/TransformComponent.h"

VerticalMovingPlatformAIComponent::VerticalMovingPlatformAIComponent(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(entityId)
    , mInitPosition(mEntityComponentManager.GetComponent<TransformComponent>(mEntityId).GetTranslation())
    , mMovingDownwards(true)
{
    
}

void VerticalMovingPlatformAIComponent::VUpdate(const float)
{
    const auto& transformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mEntityId);
    auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);
    
    if (mMovingDownwards)
    {
        physicsComponent.GetVelocity() = physicsComponent.GetM
    }
    
    
     = mMovingDownwards ? physicsComponent.GetMinVelocity() : physicsComponent.GetMaxVelocity();
    
    
}

