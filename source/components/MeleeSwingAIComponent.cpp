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
#include "../events/EntityDestroyedEvent.h"

MeleeSwingAIComponent::MeleeSwingAIComponent(const ServiceLocator& serviceLocator, const EntityId meleeSwingEntityId, const float timeToLive)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(meleeSwingEntityId)
    , mTimeToLive(timeToLive)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

void MeleeSwingAIComponent::VUpdate(const float dt)
{
    mTimeToLive -= dt;
    if (mTimeToLive <= 0.0f)
    {
        mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mEntityId));
    }
}
