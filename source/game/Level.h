//
//  Level.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 27/01/2019.
//

#ifndef Level_h
#define Level_h

#include "../util/StringId.h"
#include "../util/TypeTraits.h"

#include <vector>

class Level final
{
    friend class LevelFactory;
public:
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
    
    ~Level() = default;

private:
    Level(const std::vector<EntityNameIdEntry> mLevelEntities);
    
    std::vector<EntityNameIdEntry> mActiveEntities;
};

#endif /* Level_h */
