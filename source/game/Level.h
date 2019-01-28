//
//  Level.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 27/01/2019.
//

#ifndef Level_h
#define Level_h

#include "../util/StringId.h"
#include "GameConstants.h"

#include <vector>

class Level final
{
    friend class LevelFactory;
public:        
    ~Level() = default;

    const std::vector<EntityNameIdEntry>& GetAllActiveEntities() const;
    EntityId GetEntityIdFromName(const StringId entityName) const;

private:
    Level(const std::vector<EntityNameIdEntry> mLevelEntities);
    
    std::vector<EntityNameIdEntry> mActiveEntities;
};

#endif /* Level_h */
