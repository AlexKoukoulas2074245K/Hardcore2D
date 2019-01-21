//
//  SetEntityCustomVelocityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#include "SetEntityCustomVelocityCommand.h"

#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"


const StringId SetEntityCustomVelocityCommand::COMMAND_CLASS_ID("SetEntityCustomVelocityCommand");

SetEntityCustomVelocityCommand::SetEntityCustomVelocityCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const glm::vec3& velocity)
: mEntityComponentManager(entityComponentManager)
, mEntityId(entityId)
, mVelocity(velocity)
{
    
}

void SetEntityCustomVelocityCommand::Execute()
{
    mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId).GetVelocity() = mVelocity;
}

StringId SetEntityCustomVelocityCommand::GetCommandClassId() const
{
    return COMMAND_CLASS_ID;
}
