//
//  HorizontalMovingPlatformAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 23/02/2019.
//

#include "HorizontalMovingPlatformAIComponent.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"

const float HorizontalMovingPlatformAIComponent::MOVEMENT_DURATION_PER_DIRECTION = 4.0f;

HorizontalMovingPlatformAIComponent::HorizontalMovingPlatformAIComponent(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(entityId)    
    , mMovingRight(true)
    , mTimer(0.0f)
{
    
}

void HorizontalMovingPlatformAIComponent::VUpdate(const float dt)
{    
    auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);       

    mTimer += dt;
    if (mTimer > MOVEMENT_DURATION_PER_DIRECTION)
    {
        mTimer = 0.0f;
        mMovingRight = !mMovingRight;
    }

    if (mMovingRight)
    {
        
        physicsComponent.GetVelocity() = physicsComponent.GetMinVelocity();
    }
    else
    {
        physicsComponent.GetVelocity() = physicsComponent.GetMaxVelocity();
    }                 
}

