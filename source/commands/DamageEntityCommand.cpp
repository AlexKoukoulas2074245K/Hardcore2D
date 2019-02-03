//
//  DamageEntityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#include "DamageEntityCommand.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/HealthComponent.h"
#include "../components/DamageComponent.h"
#include "../events/EventCommunicator.h"
#include "../events/EntityDamagedEvent.h"
#include "../util/MathUtils.h"

DamageEntityCommand::DamageEntityCommand(const ServiceLocator& serviceLocator, const std::pair<EntityId, EntityId>& collidedEntityIds)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mCollidedEntities(collidedEntityIds)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

void DamageEntityCommand::Execute()
{
    // Only check first entity to avoid accounting for damage for the other collision event with reverse entity ids
    if (mEntityComponentManager.HasComponent<DamageComponent>(mCollidedEntities.first) == false)
    {
        return;
    }
    
    auto& damageComponent = mEntityComponentManager.GetComponent<DamageComponent>(mCollidedEntities.first);
    
    // Make sure the attack won't damage the owner of the attack
    if (damageComponent.GetOwnerEntityId() == mCollidedEntities.second)
    {
        return;
    }
    
    // Make sure other entity has a health component to be damaged
    if (mEntityComponentManager.HasComponent<HealthComponent>(mCollidedEntities.second) == false)
    {
        return;
    }
    
    auto& healthComponent = mEntityComponentManager.GetComponent<HealthComponent>(mCollidedEntities.second);
    
    // Make sure the other entity's health component is not temporarily invincible
    if (healthComponent.IsTemporarilyInvincible())
    {
        return;
    }
    
    // Damage entity
    healthComponent.ReduceHealthBy(damageComponent.GetDamage());
    healthComponent.SetTemporarilyInvincible(true);
    
    // Probably dispatch event
    mEventCommunicator->DispatchEvent(std::make_unique<EntityDamagedEvent>(mCollidedEntities.second, damageComponent.GetDamage(), healthComponent.GetHealth()));
}
