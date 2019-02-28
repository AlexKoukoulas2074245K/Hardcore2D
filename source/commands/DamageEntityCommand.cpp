//
//  DamageEntityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#include "DamageEntityCommand.h"
#include "../ServiceLocator.h"
#include "../components/AnimationComponent.h"
#include "../components/EntityComponentManager.h"
#include "../components/HealthComponent.h"
#include "../components/FactionComponent.h"
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

void DamageEntityCommand::VExecute()
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
    
    // Make sure the damage component can damage this entity. Don't damage if the damage component can not attack the same entity multiple times
    // and the entity has already been damaged by this component
    if (damageComponent.CanDamageSameEntityMultipleTimes() == false && damageComponent.IsEntityWhitelisted(mCollidedEntities.second))
    {
        return;
    }

    // Make sure the damage component won't damage entities of the same faction
    const auto& damageSourceEntityFaction = mEntityComponentManager.GetComponent<FactionComponent>(mCollidedEntities.first).GetFactionGroup();
    const auto& damageReceiverEntityFaction = mEntityComponentManager.GetComponent<FactionComponent>(mCollidedEntities.second).GetFactionGroup();

    if (damageSourceEntityFaction == damageReceiverEntityFaction)
    {
        return;
    }

    // Damage entity
    healthComponent.ReduceHealthBy(damageComponent.GetDamage());
    healthComponent.SetTemporarilyInvincible(true);
    
    if (damageComponent.CanDamageSameEntityMultipleTimes() == false)
    {
        damageComponent.AddEntityToWhitelistedDamagedEntities(mCollidedEntities.second);
    }
    
    mEntityComponentManager.GetComponent<AnimationComponent>(mCollidedEntities.second).PlayAnimation(StringId("hit"));
    
    mEventCommunicator->DispatchEvent(std::make_unique<EntityDamagedEvent>(mCollidedEntities.second, mCollidedEntities.first, damageComponent.GetDamage(), healthComponent.GetHealth()));
}
