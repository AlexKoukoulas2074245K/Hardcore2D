//
//  EntityDestroyedEvent.cpp
//
//  Hardcore2D
//  Created by Alex Koukoulas on 01/02/2019.
//

#include "EntityDestroyedEvent.h"

EntityDestroyedEvent::EntityDestroyedEvent(const EntityId entityId)
    : mEntityId(entityId)
{

}

EntityId EntityDestroyedEvent::GetDestroyedEntityId() const
{
    return mEntityId;
}
