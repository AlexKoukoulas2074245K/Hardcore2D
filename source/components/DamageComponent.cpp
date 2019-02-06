//
//  DamageComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 02/02/2019.
//

#include "DamageComponent.h"

#include <algorithm>

DamageComponent::DamageComponent(const EntityId ownerEntityId, const float damage, const bool canDamageSameEntityMultipleTimes)
    : mOwnerEntityId(ownerEntityId)
    , mDamage(damage)
    , mCanDamageSameEntityMultipleTimes(canDamageSameEntityMultipleTimes)
{
    
}

EntityId DamageComponent::GetOwnerEntityId() const
{
    return mOwnerEntityId;
}

float DamageComponent::GetDamage() const
{
    return mDamage;
}

bool DamageComponent::CanDamageSameEntityMultipleTimes() const
{
    return mCanDamageSameEntityMultipleTimes;
}

bool DamageComponent::IsEntityWhitelisted(const EntityId entityId) const
{
    return mWhitelistedDamagedEntities.count(entityId) != 0;
}

void DamageComponent::AddEntityToWhitelistedDamagedEntities(const EntityId entityId)
{
    mWhitelistedDamagedEntities.insert(entityId);
}
