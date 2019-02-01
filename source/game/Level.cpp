//
//  Level.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 27/01/2019.
//

#include "Level.h"
#include "../ServiceLocator.h"
#include "../events/EventCommunicator.h"
#include "../events/NewEntityCreatedEvent.h"

#include <algorithm>
#include <cassert>

Level::Level(const ServiceLocator& serviceLocator, const std::vector<EntityNameIdEntry> mLevelEntities, const glm::vec2& horizontalBounds, const glm::vec2& verticalBounds)
    : mActiveEntities(mLevelEntities)
    , mLevelHorizontalBounds(horizontalBounds)
    , mLevelVerticalBounds(verticalBounds)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    mEventCommunicator->RegisterEventCallback<NewEntityCreatedEvent>([this](const IEvent& event)
    {
        mActiveEntities.push_back(static_cast<const NewEntityCreatedEvent&>(event).GetNewEntityNameIdEntry());
    });
}

Level::~Level()
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
