//
//  DamageEntityCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#ifndef DamageEntityCommand_h
#define DamageEntityCommand_h

#include "ICommand.h"
#include "../util/TypeTraits.h"

#include <memory>
#include <utility>

class EntityComponentManager;
class EventCommunicator;
class ServiceLocator;
class HealthComponent;

class DamageEntityCommand final: public ICommand
{
public:
    DamageEntityCommand(const ServiceLocator&, const std::pair<EntityId, EntityId>& collidedEntityIds);
    
    void Execute() override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    const std::pair<EntityId, EntityId> mCollidedEntities;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
};

#endif /* DamageEntityCommand_h */
