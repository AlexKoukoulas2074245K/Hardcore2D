//
//  NewEntityCreatedEvent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 01/02/2019.
//

#include "NewEntityCreatedEvent.h"

NewEntityCreatedEvent::NewEntityCreatedEvent(const EntityId entityId)
    : mNewEntityId(entityId)
{
    
}

EntityId NewEntityCreatedEvent::GetNewEntityId() const
{
    return mNewEntityId;
}
