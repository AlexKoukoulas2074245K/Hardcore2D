//
//  RangedShurikenAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 05/02/2019.
//

#include "RangedShurikenAIComponent.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../commands/SetVelocityAndAnimateCommand.h"
#include "../commands/SetAngularVelocityCommand.h"
#include "../events/EntityCollisionEvent.h"
#include "../events/EventCommunicator.h"
#include "../events/EntityDamagedEvent.h"
#include "../events/EntityDestroyedEvent.h"

RangedShurikenAIComponent::RangedShurikenAIComponent(const ServiceLocator& serviceLocator, const EntityId entityId, const float timeToLive)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(entityId)
    , mTimeToLiveTimer(timeToLive, [this](){ OnTimeToLiveTimerTick(); })
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    mEventCommunicator->RegisterEventCallback<EntityDamagedEvent>([this](const IEvent& event)
    {
        const auto& actualEvent = static_cast<const EntityDamagedEvent&>(event);
        const auto damageSenderEntityId = actualEvent.GetDamageSenderEntityId();
        if (damageSenderEntityId == mEntityId)
        {
            mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mEntityId));
        }
    });
    mEventCommunicator->RegisterEventCallback<EntityCollisionEvent>([this](const IEvent& event)
    {
        const auto collidingEntityIds = static_cast<const EntityCollisionEvent&>(event).GetCollidedEntityIds();
        
        if (collidingEntityIds.first == mEntityId && mEntityComponentManager.GetComponent<PhysicsComponent>(collidingEntityIds.second).GetBodyType() != PhysicsComponent::BodyType::DYNAMIC)
        {
            mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mEntityId));
        }
    });
}

void RangedShurikenAIComponent::VUpdate(const float dt)
{
    mTimeToLiveTimer.Update(dt);
}

void RangedShurikenAIComponent::OnTimeToLiveTimerTick()
{
    mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mEntityId));
}
