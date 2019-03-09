//
//  FlameBreathAttackCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 09/03/2019.
//

#ifndef FlameBreathAttackCommand_h
#define FlameBreathAttackCommand_h

#include "ICommand.h"
#include "../util/TypeTraits.h"

#include <memory>

class EntityComponentManager;
class ServiceLocator;
class EventCommunicator;

class FlameBreathAttackCommand final: public ICommand
{
public:
    FlameBreathAttackCommand(const ServiceLocator&, const EntityId);
    void VExecute() override;
    
private:
    const ServiceLocator& mServiceLocator;
    EntityComponentManager& mEntityComponentManager;
    const EntityId mParentEntityId;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
};


#endif /* FlameBreathAttackCommand_h */
