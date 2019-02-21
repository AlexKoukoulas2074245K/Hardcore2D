//
//  GameTypeTraits.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 26/01/2019.
//

#ifndef GameTypeTraits_h
#define GameTypeTraits_h

#include "../util/StringId.h"

struct EntityNameIdEntry
{
    EntityNameIdEntry(const StringId entityName, const EntityId entityId)
        : mEntityName(entityName)
        , mEntityId(entityId)
    {
    };

    operator size_t () { return static_cast<size_t>(mEntityId); }
    bool operator == (const EntityNameIdEntry& other) const { return mEntityId == other.mEntityId; }

    StringId mEntityName;
    EntityId mEntityId;
};

struct EntityNameIdEntryHasher
{
    std::size_t operator()(const EntityNameIdEntry& key) const
    {
        return static_cast<std::size_t>(key.mEntityId);
    }
};

#endif /* GameTypeTraits_h */
