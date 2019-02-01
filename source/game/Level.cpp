//
//  Level.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 27/01/2019.
//

#include "Level.h"

#include <algorithm>
#include <cassert>

Level::Level(const std::vector<EntityNameIdEntry> mLevelEntities, const glm::vec2& horizontalBounds, const glm::vec2& verticalBounds)
    : mActiveEntities(mLevelEntities)
    , mLevelHorizontalBounds(horizontalBounds)
    , mLevelVerticalBounds(verticalBounds)
{
}

EntityId Level::GetEntityIdFromName(const StringId entityName) const
{
    const auto resultIter = std::find_if(mActiveEntities.begin(), mActiveEntities.end(), [entityName](const EntityNameIdEntry& entityEntry)
    {
        return entityEntry.mEntityName == entityName;
    });
    
    if (resultIter == mActiveEntities.end())
    {
        assert(false);
    }
    
    return resultIter->mEntityId;
}

void Level::AddEntityNameIdEntryToActiveEntities(const EntityNameIdEntry& entityNameIdEntry)
{
    mActiveEntities.push_back(entityNameIdEntry);
}

const std::vector<EntityNameIdEntry>& Level::GetAllActiveEntities() const
{
    return mActiveEntities;
}

const glm::vec2& Level::GetHorizontalBounds() const
{
    return mLevelHorizontalBounds;
}

const glm::vec2& Level::GetVerticalBounds() const
{
    return mLevelVerticalBounds;
}
