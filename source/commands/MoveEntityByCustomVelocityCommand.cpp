//
//  MoveEntityByCustomVelocityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "MoveEntityByCustomVelocityCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"


const StringId MoveEntityByCustomVelocityCommand::COMMAND_CLASS_ID("MoveEntityByCustomVelocityCommand");

MoveEntityByCustomVelocityCommand::MoveEntityByCustomVelocityCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const glm::vec3& velocity)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
    , mVelocity(velocity)
{
    
}

void MoveEntityByCustomVelocityCommand::Execute()
{
    mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId).GetVelocity() += mVelocity;
}

StringId MoveEntityByCustomVelocityCommand::GetCommandClassId() const
{
	return COMMAND_CLASS_ID;
}
