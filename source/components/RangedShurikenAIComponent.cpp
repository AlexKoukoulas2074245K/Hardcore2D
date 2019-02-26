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
#include "../commands/SetEntityCustomVelocityCommand.h"
#include "../events/EntityCollisionEvent.h"
#include "../events/EventCommunicator.h"
#include "../events/EntityDamagedEvent.h"
#include "../events/EntityDestroyedEvent.h"

RangedShurikenAIComponent::RangedShurikenAIComponent(const ServiceLocator& serviceLocator, const EntityId entityId, const float timeToLive)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(entityId)
    , mTimeToLive(timeToLive)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    mEventCommunicator->RegisterEventCallback<EntityDamagedEvent>([this](const IEvent& event)
    {
        const auto& actualEvent = static_cast<const EntityDamagedEvent&>(event);
        const auto damageSenderEntityId = actualEvent.GetDamageSenderEntityId();
        const auto damagedEntityId = actualEvent.GetDamagedEntityId();
        
        if (damageSenderEntityId == mEntityId)
        {
            SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, 0.0f, 0.0f)).VExecute();
            mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId).GetAngularVelocity() = 0.0f;
            mTimeToLive = 0.5f;
            
            auto& thisTransformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mEntityId);
            const auto& otherEntityTransformComponent = mEntityComponentManager.GetComponent<TransformComponent>(damagedEntityId);
            
            thisTransformComponent.SetParent(damagedEntityId, thisTransformComponent.GetTranslation() - otherEntityTransformComponent.GetTranslation());
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
    mTimeToLive -= dt;
    if (mTimeToLive <= 0.0f)
    {
        mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mEntityId));
    }
}
