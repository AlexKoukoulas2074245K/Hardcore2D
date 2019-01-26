//
//  MoveEntityByCustomVelocityCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#ifndef MoveEntityByCustomVelocityCommand_h
#define MoveEntityByCustomVelocityCommand_h

#include "ICommand.h"
#include "../util/TypeTraits.h"

#include <glm/vec3.hpp>
#include <memory>

class ServiceLocator;
class EntityComponentManager;
class EventCommunicator;

class MoveEntityByCustomVelocityCommand final: public ICommand
{
public:
	static const StringId COMMAND_CLASS_ID;

    MoveEntityByCustomVelocityCommand(const ServiceLocator&, const EntityId, const glm::vec3& velocity);
    void Execute() override;
	StringId GetCommandClassId() const override;

private:
    EntityComponentManager& mEntityComponentManager;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
    const EntityId mEntityId;
    const glm::vec3 mVelocity;
};

#endif /* MoveEntityByCustomVelocityCommand_h */
