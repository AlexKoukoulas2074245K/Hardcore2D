//
//  BloodDropAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/02/2019.
//

#include "BloodDropAIComponent.h"
#include "../ServiceLocator.h"
#include "../commands/SetVelocityCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/TransformComponent.h"
#include "../events/EventCommunicator.h"
#include "../events/EntityDestroyedEvent.h"
#include "../events/EntityCollisionEvent.h"
#include "../util/MathUtils.h"

BloodDropAIComponent::BloodDropAIComponent(const ServiceLocator& serviceLocator, const EntityId bloodDropEntityId, const float timeToLive)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(bloodDropEntityId)
    , mTimeToLive(timeToLive)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{

}

void BloodDropAIComponent::VUpdate(const float dt)
{
    const auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);
    auto& transformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mEntityId);
    
    if (glm::length(physicsComponent.GetVelocity()) < 0.01f)
    {
        transformComponent.GetRotation().z = PI*1.5f;
    }
    else
    {
        transformComponent.GetRotation().z = PI*0.5f + Arctan2(-physicsComponent.GetVelocity().x, physicsComponent.GetVelocity().y);
    }
    
    
    mTimeToLive -= dt;
    if (mTimeToLive <= 0.0f)
    {
        mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mEntityId));
    }
}
