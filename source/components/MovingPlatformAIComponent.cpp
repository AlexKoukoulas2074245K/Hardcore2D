//
//  MovingPlatformAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 23/02/2019.
//

#include "MovingPlatformAIComponent.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"

const float MovingPlatformAIComponent::MOVEMENT_DURATION_PER_DIRECTION = 4.0f;

MovingPlatformAIComponent::MovingPlatformAIComponent(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(entityId)    
    , mMovingNorthOrEastOrBoth(true)
    , mTimer(0.0f)
{
    
}

void MovingPlatformAIComponent::VUpdate(const float dt)
{    
    auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);       

    mTimer += dt;
    if (mTimer > MOVEMENT_DURATION_PER_DIRECTION)
    {
        mTimer = 0.0f;
        mMovingNorthOrEastOrBoth = !mMovingNorthOrEastOrBoth;
    }

    if (mMovingNorthOrEastOrBoth)
    {
        
        physicsComponent.GetVelocity() = physicsComponent.GetMaxVelocity();
    }
    else
    {
        physicsComponent.GetVelocity() = physicsComponent.GetMinVelocity();
    }                 
}

