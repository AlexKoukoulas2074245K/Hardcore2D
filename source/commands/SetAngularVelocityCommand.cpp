//
//  SetAngularVelocityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 04/03/2019.
//

#include "SetAngularVelocityCommand.h"
#include "../components/PhysicsComponent.h"
#include "../components/EntityComponentManager.h"

SetAngularVelocityCommand::SetAngularVelocityCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const float angularVelocity)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
    , mAngularVelocity(angularVelocity)
{
    
}

void SetAngularVelocityCommand::VExecute()
{
    mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId).mAngularVelocity = mAngularVelocity;
}
