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
#include "GameTypeTraits.h"

#include <list>
#include <vector>
#include <memory>

class ServiceLocator;
class EntityComponentManager;
class EventCommunicator;

class Level final
{
    friend class LevelFactory;
public:        
    ~Level();   
    
    EntityId GetEntityIdFromName(const StringId entityName) const;
    void CheckForAdditionsOrRemovalsOfEntities();

    const std::vector<EntityNameIdEntry>& GetAllActiveEntities() const;
    const glm::vec2& GetHorizontalBounds() const;
    const glm::vec2& GetVerticalBounds() const;
    
private:
    Level(const ServiceLocator&, const std::vector<EntityNameIdEntry> mLevelEntities, const glm::vec2& horizontalBounds, const glm::vec2& verticalBounds);

    void RemoveEntityFromActiveEntities(const EntityId entityId);

    EntityComponentManager& mEntityComponentManager;

    std::list<EntityNameIdEntry> mEntityIdsToAdd;
    std::list<EntityId> mEntityIdsToRemove;

    std::vector<EntityNameIdEntry> mActiveEntities;
    const glm::vec2 mLevelHorizontalBounds;
    const glm::vec2 mLevelVerticalBounds;
    
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
};

#endif /* Level_h */
