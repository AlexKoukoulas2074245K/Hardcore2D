//
//  MoveEntityByCustomVelocityCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "MoveEntityByCustomVelocityCommand.h"
#include "../ServiceLocator.h"
#include "../events/PlayerChangedDirectionEvent.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/ShaderComponent.h"
#include "../util/MathUtils.h"

const StringId MoveEntityByCustomVelocityCommand::COMMAND_CLASS_ID("MoveEntityByCustomVelocityCommand");

MoveEntityByCustomVelocityCommand::MoveEntityByCustomVelocityCommand(const ServiceLocator& serviceLocator, const EntityId entityId, const glm::vec3& velocity)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())    
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

        animationComponent.ChangeAnimation(StringId("running"));       
    }
            
    physicsComponent.GetVelocity() += mVelocity;
}

StringId MoveEntityByCustomVelocityCommand::GetCommandClassId() const
{
	return COMMAND_CLASS_ID;
}
