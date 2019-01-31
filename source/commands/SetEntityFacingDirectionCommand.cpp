//
//  SetEntityFacingDirectionCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#include "SetEntityFacingDirectionCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/AnimationComponent.h"

const StringId SetEntityFacingDirectionCommand::COMMAND_CLASS_ID("SetEntityFacingDirectionCommand");

SetEntityFacingDirectionCommand::SetEntityFacingDirectionCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const FacingDirection facingDirection)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
    , mFacingDirection(facingDirection)
{
    
}

void SetEntityFacingDirectionCommand::Execute()
{
    mEntityComponentManager.GetComponent<AnimationComponent>(mEntityId).SetFacingDirection(mFacingDirection);
}

StringId SetEntityFacingDirectionCommand::GetCommandClassId() const
{
    return COMMAND_CLASS_ID;
}
