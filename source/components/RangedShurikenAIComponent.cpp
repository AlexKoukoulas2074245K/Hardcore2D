//
//  RangedShurikenAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 05/02/2019.
//

#include "RangedShurikenAIComponent.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../events/EventCommunicator.h"
#include "../events/EntityDestroyedEvent.h"

RangedShurikenAIComponent::RangedShurikenAIComponent(const ServiceLocator& serviceLocator, const EntityId meleeSwingEntityId, const float timeToLive)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(meleeSwingEntityId)
    , mTimeToLive(timeToLive)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

void RangedShurikenAIComponent::VUpdate(const float dt)
{
    mTimeToLive -= dt;
    if (mTimeToLive <= 0.0f)
    {
        mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mEntityId));
    }
}
