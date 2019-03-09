//
//  MovingPlatformAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 23/02/2019.
//

#include "MovingPlatformAIComponent.h"
#include "../ServiceLocator.h"
#include "../commands/SetVelocityCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../util/MathUtils.h"

const float MovingPlatformAIComponent::MOVEMENT_DURATION_PER_DIRECTION = 4.0f;

MovingPlatformAIComponent::MovingPlatformAIComponent(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(entityId)    
    , mMovingNorthOrEastOrBoth(true)
    , mMovementTimer(RandomFloat(0.0f, MOVEMENT_DURATION_PER_DIRECTION), [this](){ OnMovementTimerTick(); })
{
    
}

void MovingPlatformAIComponent::VUpdate(const float dt)
{    
    auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);       

    mMovementTimer.Update(dt);

    if (mMovingNorthOrEastOrBoth)
    {
        
        SetVelocityCommand(mEntityComponentManager, mEntityId, physicsComponent.GetMaxVelocity()).VExecute();
    }
    else
    {
        SetVelocityCommand(mEntityComponentManager, mEntityId, physicsComponent.GetMinVelocity()).VExecute();
    }                 
}

void MovingPlatformAIComponent::OnMovementTimerTick()
{
    mMovingNorthOrEastOrBoth = !mMovingNorthOrEastOrBoth;
}
