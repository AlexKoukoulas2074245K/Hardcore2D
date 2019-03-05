//
//  SetFacingDirectionCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#include "SetFacingDirectionCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/AnimationComponent.h"

SetFacingDirectionCommand::SetFacingDirectionCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const FacingDirection facingDirection)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
    , mFacingDirection(facingDirection)
{
    
}

void SetFacingDirectionCommand::VExecute()
{
    mEntityComponentManager.GetComponent<AnimationComponent>(mEntityId).SetFacingDirection(mFacingDirection);
}

