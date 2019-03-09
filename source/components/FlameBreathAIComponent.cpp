//
//  FlameBreathAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 09/03/2019.
//

#include "FlameBreathAIComponent.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../events/EventCommunicator.h"
#include "../events/EntityDestroyedEvent.h"

FlameBreathAIComponent::FlameBreathAIComponent(const ServiceLocator& serviceLocator, const EntityId flameBreathEntityId, const float timeToLive)
: mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
, mEntityId(flameBreathEntityId)
, mTimeToLiveTimer(timeToLive, [this](){ OnTimeToLiveTimerTick(); })
, mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

void FlameBreathAIComponent::VUpdate(const float dt)
{
    mTimeToLiveTimer.Update(dt);
}

void FlameBreathAIComponent::OnTimeToLiveTimerTick()
{
    mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mEntityId));
}
