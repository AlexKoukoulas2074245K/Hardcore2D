//
//  Level.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 27/01/2019.
//

#include "Level.h"

#include <algorithm>
#include <cassert>

Level::Level(const std::vector<EntityNameIdEntry> mLevelEntities)
    : mActiveEntities(mLevelEntities)
{
}

const std::vector<EntityNameIdEntry>& Level::GetAllActiveEntities() const
{
    return mActiveEntities;
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