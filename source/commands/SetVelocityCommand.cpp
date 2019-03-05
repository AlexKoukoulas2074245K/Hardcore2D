//
//  SetVelocityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#include "SetVelocityCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"

SetVelocityCommand::SetVelocityCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const float x, const float y, const float z)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
    , mVelocity(glm::vec3(x, y, z))
{

}

SetVelocityCommand::SetVelocityCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const glm::vec3& velocity)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
    , mVelocity(velocity)
{
    
}

void SetVelocityCommand::VExecute()
{    
    mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId).mVelocity = mVelocity;
}
