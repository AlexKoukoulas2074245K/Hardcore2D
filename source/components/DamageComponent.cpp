//
//  DamageComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 02/02/2019.
//

#include "DamageComponent.h"

#include <algorithm>

DamageComponent::DamageComponent(const EntityId ownerEntityId, const float damage)
    : mOwnerEntityId(ownerEntityId)
    , mDamage(damage)
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
