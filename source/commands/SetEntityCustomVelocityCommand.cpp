//
//  SetEntityCustomVelocityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#include "SetEntityCustomVelocityCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"
#include "../util/MathUtils.h"

const StringId SetEntityCustomVelocityCommand::COMMAND_CLASS_ID("SetEntityCustomVelocityCommand");

SetEntityCustomVelocityCommand::SetEntityCustomVelocityCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const glm::vec3& velocity)
: mEntityComponentManager(entityComponentManager)
, mEntityId(entityId)
, mVelocity(velocity)
{
    
}

void SetEntityCustomVelocityCommand::Execute()
{
    if (mEntityComponentManager.HasComponent<AnimationComponent>(mEntityId))
    {
        auto& animationComponent = mEntityComponentManager.GetComponent<AnimationComponent>(mEntityId);
        if (Abs(mVelocity.x) < 1.0f)
        {
            animationComponent.PlayAnimation(StringId("idle"));
        }
        else
        {
            if (mVelocity.x < 0.0f)
            {
                if (animationComponent.GetCurrentFacingDirection() != FacingDirection::LEFT)
                {
                    animationComponent.SetFacingDirection(FacingDirection::LEFT);
                }
            }
            else
            {
                if (animationComponent.GetCurrentFacingDirection() != FacingDirection::RIGHT)
                {
                    animationComponent.SetFacingDirection(FacingDirection::RIGHT);
                }
            }
            
            animationComponent.PlayAnimation(StringId("running"));
        }
        
    }
    mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId).GetVelocity() = mVelocity;
}

StringId SetEntityCustomVelocityCommand::GetCommandClassId() const
{
    return COMMAND_CLASS_ID;
}
