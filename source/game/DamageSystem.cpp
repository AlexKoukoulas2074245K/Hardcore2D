//
//  DamageSystem.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 02/02/2019.
//

#include "DamageSystem.h"
#include "../ServiceLocator.h"
#include "../events/EventCommunicator.h"
#include "../events/EntityCollisionEvent.h"
#include "../commands/DamageEntityCommand.h"
#include "../components/HealthComponent.h"
#include "../components/EntityComponentManager.h"

DamageSystem::DamageSystem(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
    
}

bool DamageSystem::VInitialize()
{
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
    mEventCommunicator = mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    
    mEventCommunicator->RegisterEventCallback<EntityCollisionEvent>([this](const IEvent& event)
    {
        OnEntityCollisionEvent(event);
    });
    
    return true;
}

void DamageSystem::Update(const std::vector<EntityNameIdEntry>& activeEntities, const float dt)
{
    for (const auto& entityNameIdEntry: activeEntities)
    {
        if (mEntityComponentManager->HasComponent<HealthComponent>(entityNameIdEntry.mEntityId))
        {
            auto& healthComponent = mEntityComponentManager->GetComponent<HealthComponent>(entityNameIdEntry.mEntityId);
            
            if (healthComponent.IsTemporarilyInvincible())
            {
                // Update invincibility 
                healthComponent.SetInvincibilityTimer(healthComponent.GetInvincibilityTimer() + dt);
                
                // Check whether invincibility timer has exceeded the invincibility duration in which case it must be reset
                if (healthComponent.GetInvincibilityTimer() > healthComponent.GetInvincibilityDuration())
                {
                    healthComponent.SetInvincibilityTimer(0.0f);
                    healthComponent.SetTemporarilyInvincible(false);
                }
            }
        }
    }
}

void DamageSystem::OnEntityCollisionEvent(const IEvent& event)
{
    const auto& collidedEntities = static_cast<const EntityCollisionEvent&>(event).GetCollidedEntityIds();
    DamageEntityCommand(mServiceLocator, collidedEntities).Execute();
}

