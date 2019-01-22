//
//  MoveEntityByCustomVelocityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "MoveEntityByCustomVelocityCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/ShaderComponent.h"
#include "../util/MathUtils.h"

const StringId MoveEntityByCustomVelocityCommand::COMMAND_CLASS_ID("MoveEntityByCustomVelocityCommand");

MoveEntityByCustomVelocityCommand::MoveEntityByCustomVelocityCommand(EntityComponentManager& entityComponentManager, const EntityId entityId, const glm::vec3& velocity)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
    , mVelocity(velocity)
{
    
}

void MoveEntityByCustomVelocityCommand::Execute()
{
    auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);
    if (mEntityComponentManager.HasComponent<AnimationComponent>(mEntityId))
    {
        auto& animationComponent = mEntityComponentManager.GetComponent<AnimationComponent>(mEntityId);
        if (mVelocity.x < 0.0f)
        {
            animationComponent.SetFacingDirection(AnimationComponent::FacingDirection::LEFT);
        }
        else
        {
            animationComponent.SetFacingDirection(AnimationComponent::FacingDirection::RIGHT);
        }
        
        if (animationComponent.GetCurrentAnimation() != StringId("running"))
        {
            animationComponent.ChangeAnimation(StringId("running"));
        }
    }
            
    physicsComponent.GetVelocity() += mVelocity;
}

StringId MoveEntityByCustomVelocityCommand::GetCommandClassId() const
{
	return COMMAND_CLASS_ID;
}
