//
//  NewEntityCreatedEvent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 01/02/2019.
//

#include "NewEntityCreatedEvent.h"

NewEntityCreatedEvent::NewEntityCreatedEvent(const EntityNameIdEntry& entityNameIdEntry)
    : mEntityNameIdEntry(entityNameIdEntry)
{
    
}

const EntityNameIdEntry& NewEntityCreatedEvent::GetNewEntityNameIdEntry() const
{
    return mEntityNameIdEntry;
}
