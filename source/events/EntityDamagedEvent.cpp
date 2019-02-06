//
//  EntityDamagedEvent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#include "EntityDamagedEvent.h"


EntityDamagedEvent::EntityDamagedEvent(const EntityId damagedEntityId, const EntityId damageSenderEntityId, const float damageDone, const float healthRemaining)
    : mDamagedEntityId(damagedEntityId)
    , mDamageSenderEntityId(damageSenderEntityId)
    , mDamageDone(damageDone)
    , mHealthRemaining(healthRemaining)
{
    
}

EntityId EntityDamagedEvent::GetDamagedEntityId() const
{
    return mDamagedEntityId;
}

EntityId EntityDamagedEvent::GetDamageSenderEntityId() const
{
    return mDamageSenderEntityId;
}

float EntityDamagedEvent::GetDamageDone() const
{
    return mDamageDone;
}

float EntityDamagedEvent::GetHealthRemaining() const
{
    return mHealthRemaining;
}
