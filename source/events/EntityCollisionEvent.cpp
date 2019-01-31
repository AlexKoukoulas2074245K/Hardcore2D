//
//  EntityCollisionEvent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#include "EntityCollisionEvent.h"

EntityCollisionEvent::EntityCollisionEvent(const std::pair<EntityId, EntityId>& collidedEntityIds)
    : mCollidedEntityIds(collidedEntityIds)
{
    
}

const std::pair<EntityId, EntityId>& EntityCollisionEvent::GetCollidedEntityIds() const
{
    return mCollidedEntityIds;
}
