//
//  MeleeSwingAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 01/02/2019.
//

#include "MeleeSwingAIComponent.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../events/EventCommunicator.h"

MeleeSwingAIComponent::MeleeSwingAIComponent(const ServiceLocator& serviceLocator, const EntityId meleeSwingEntityId, const float timeToLive)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(meleeSwingEntityId)
    , mTimeToLive(timeToLive)
{
    
}

void MeleeSwingAIComponent::VUpdate(const float dt)
{
    
}
