//
//  EntityRangedShurikenAttackCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#ifndef EntityRangedShurikenAttackCommand_h
#define EntityRangedShurikenAttackCommand_h

#include "ICommand.h"
#include "../util/TypeTraits.h"

#include <memory>

class ServiceLocator;
class EntityComponentManager;
class ResourceManager;
class EventCommunicator;

class EntityRangedShurikenAttackCommand final: public ICommand
{
public:
    EntityRangedShurikenAttackCommand(const ServiceLocator&, const EntityId);
    
    void VExecute() override;
private:
    const ServiceLocator& mServiceLocator;
    EntityComponentManager& mEntityComponentManager;
    ResourceManager& mResourceManager;    
    const EntityId mParentEntityId;
    
    std::unique_ptr<EventCommunicator> mEventCommunicator;
};

#endif /* EntityRangedShurikenAttackCommand_h */
