//
//  EntityMeleeAttackCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#ifndef EntityMeleeAttackCommand_h
#define EntityMeleeAttackCommand_h

#include "ICommand.h"
#include "../util/TypeTraits.h"

#include <memory>

class EntityComponentManager;
class ResourceManager;
class ServiceLocator;
class EventCommunicator;

class EntityMeleeAttackCommand final: public ICommand
{
public:
    EntityMeleeAttackCommand(const ServiceLocator&, const EntityId);
    void VExecute() override;
    
private:
    const ServiceLocator& mServiceLocator;
    EntityComponentManager& mEntityComponentManager;
    ResourceManager& mResourceManager;
    const EntityId mParentEntityId;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
};


#endif /* EntityMeleeAttackCommand_h */
