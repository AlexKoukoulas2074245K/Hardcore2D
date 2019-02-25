//
//  VerticalMovingPlatformAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 23/02/2019.
//

#include "VerticalMovingPlatformAIComponent.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"

const float VerticalMovingPlatformAIComponent::MOVEMENT_DURATION_PER_DIRECTION = 4.0f;

VerticalMovingPlatformAIComponent::VerticalMovingPlatformAIComponent(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(entityId)    
    , mMovingDownwards(true)
    , mTimer(0.0f)
{
    
}

void VerticalMovingPlatformAIComponent::VUpdate(const float dt)
{    
    auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);       

    mTimer += dt;
    if (mTimer > MOVEMENT_DURATION_PER_DIRECTION)
    {
        mTimer = 0.0f;
        mMovingDownwards = !mMovingDownwards;
    }

    if (mMovingDownwards)
    {
        
        physicsComponent.GetVelocity() = physicsComponent.GetMinVelocity();
    }
    else
    {
        physicsComponent.GetVelocity() = physicsComponent.GetMaxVelocity();
    }                 
}

