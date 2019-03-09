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
#include "../events/FlameBreathFinishedEvent.h"

FlameBreathAIComponent::FlameBreathAIComponent(const ServiceLocator& serviceLocator, const EntityId parentEntityId, const EntityId flameBreathEntityId, const float timeToLive)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mParentEntityId(parentEntityId)
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
    mEventCommunicator->DispatchEvent(std::make_unique<FlameBreathFinishedEvent>(mParentEntityId));
    mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mEntityId));
}
