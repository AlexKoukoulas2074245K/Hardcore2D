//
//  SetEntityVelocityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "SetEntityVelocityCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"

SetEntityVelocityCommand::SetEntityVelocityCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const glm::vec3& velocity)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
    , mVelocity(velocity)
{
    
}

void SetEntityVelocityCommand::Execute()
{
    mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId).GetVelocity() = mVelocity;
}
