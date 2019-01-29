//
//  GameConstants.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 26/01/2019.
//

#ifndef GameConstants_h
#define GameConstants_h

#include "../util/StringId.h"

enum class FacingDirection
{
    LEFT, RIGHT
};

struct EntityNameIdEntry
{
    EntityNameIdEntry(const StringId entityName, const EntityId entityId)
        : mEntityName(entityName)
        , mEntityId(entityId)
    {
    };

    StringId mEntityName;
    EntityId mEntityId;
};

#endif /* GameConstants_h */
