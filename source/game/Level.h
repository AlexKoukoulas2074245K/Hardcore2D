//
//  Level.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 27/01/2019.
//

#ifndef Level_h
#define Level_h

#include "../util/StringId.h"
#include "../util/MathUtils.h"
#include "GameConstants.h"

#include <vector>

class Level final
{
    friend class LevelFactory;
public:        
    ~Level() = default;
    
    
    EntityId GetEntityIdFromName(const StringId entityName) const;
    void AddEntityNameIdEntryToActiveEntities(const EntityNameIdEntry&);
    
    const std::vector<EntityNameIdEntry>& GetAllActiveEntities() const;
    const glm::vec2& GetHorizontalBounds() const;
    const glm::vec2& GetVerticalBounds() const;
    
private:
    Level(const std::vector<EntityNameIdEntry> mLevelEntities, const glm::vec2& horizontalBounds, const glm::vec2& verticalBounds);
    
    std::vector<EntityNameIdEntry> mActiveEntities;
    const glm::vec2 mLevelHorizontalBounds;
    const glm::vec2 mLevelVerticalBounds;
};

#endif /* Level_h */
