//
//  SetEntityMovementVelocityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "SetEntityMovementVelocityCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"


const StringId SetEntityMovementVelocityCommand::COMMAND_CLASS_ID("SetEntityMovementVelocityCommand");

SetEntityMovementVelocityCommand::SetEntityMovementVelocityCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const glm::vec3& velocity)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
    , mVelocity(velocity)
{
    
}

void SetEntityMovementVelocityCommand::Execute()
{
    mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId).GetVelocity() = mVelocity;
}

StringId SetEntityMovementVelocityCommand::GetCommandClassId() const
{
	return COMMAND_CLASS_ID;
}