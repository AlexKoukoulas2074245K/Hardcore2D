//
//  Level.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 27/01/2019.
//

#include "Level.h"
#include "../ServiceLocator.h"
#include "../events/EventCommunicator.h"
#include "../components/EntityComponentManager.h"
#include "../events/NewEntityCreatedEvent.h"
#include "../events/EntityDestroyedEvent.h"
#include "../util/Logging.h"

#include <algorithm>
#include <cassert>

Level::Level(const ServiceLocator& serviceLocator, const std::vector<EntityNameIdEntry> mLevelEntities, const glm::vec2& horizontalBounds, const glm::vec2& verticalBounds)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mActiveEntities(mLevelEntities)
    , mLevelHorizontalBounds(horizontalBounds)
    , mLevelVerticalBounds(verticalBounds)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    mEventCommunicator->RegisterEventCallback<NewEntityCreatedEvent>([this](const IEvent& event)
    {
        mEntityIdsToAdd.push_back(static_cast<const NewEntityCreatedEvent&>(event).GetNewEntityNameIdEntry());        
    });
    mEventCommunicator->RegisterEventCallback<EntityDestroyedEvent>([this](const IEvent& event)
    {
        mEntityIdsToRemove.push_back(static_cast<const EntityDestroyedEvent&>(event).GetDestroyedEntityId());
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

void Level::CheckForAdditionsOrRemovalsOfEntities()
{    
    while (!mEntityIdsToRemove.empty())
    {
        const auto destroyedEntityId = mEntityIdsToRemove.front();
        mEntityComponentManager.RemoveEntityEntry(destroyedEntityId);
        RemoveEntityFromActiveEntities(destroyedEntityId);
        mEntityIdsToRemove.pop_front();
    }

    while (!mEntityIdsToAdd.empty())
    {
        mActiveEntities.push_back(mEntityIdsToAdd.front());
        mEntityIdsToAdd.pop_front();
    }
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

void Level::RemoveEntityFromActiveEntities(const EntityId entityId)
{
    auto entityIter = mActiveEntities.begin();
    while (entityIter != mActiveEntities.end())
    {
        if (entityIter->mEntityId == entityId)
        {
            entityIter = mActiveEntities.erase(entityIter);
        }
        else
        {
            entityIter++;
        }
    }
}
