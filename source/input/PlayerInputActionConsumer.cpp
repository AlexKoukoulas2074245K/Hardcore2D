//
//  PlayerInputActionConsumer.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "PlayerInputActionConsumer.h"
#include "../components/PhysicsComponent.h"
#include "../components/EntityComponentManager.h"
#include "../commands/MoveEntityByCustomVelocityCommand.h"
#include "../commands/SetEntityCustomVelocityCommand.h"
#include "../util/Logging.h"

#include <glm/glm.hpp>

PlayerInputActionConsumer::PlayerInputActionConsumer(EntityComponentManager& entityComponentManager, const EntityId entityId)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
{
    
}

bool PlayerInputActionConsumer::VConsumeInputAction(const InputAction& inputAction) const
{
    auto& entityPhysicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);
    switch (inputAction.mActionType)
    {
        case InputAction::ActionType::MOVE_LEFT:
        {
            switch (inputAction.mActionState)
            {
                case InputAction::ActionState::START:
				case InputAction::ActionState::CONTINUE:
                {
                    MoveEntityByCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(-entityPhysicsComponent.GetMaxVelocity().x, 0.0f, 0.0f)).Execute();
                } break;
                case InputAction::ActionState::STOP:
                {
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();
                } break;
            }
                        
        } break;
        case InputAction::ActionType::MOVE_RIGHT:
        {            
            switch (inputAction.mActionState)
            {
                case InputAction::ActionState::START:
                case InputAction::ActionState::CONTINUE:
                {
                    MoveEntityByCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(entityPhysicsComponent.GetMaxVelocity().x, 0.0f, 0.0f)).Execute();
                } break;
                case InputAction::ActionState::STOP:
                {
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();
                } break;
            }
                        
        } break;
        case InputAction::ActionType::JUMP:
        {
            switch (inputAction.mActionState)
            {
                case InputAction::ActionState::START:
                {   
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(entityPhysicsComponent.GetVelocity().x, entityPhysicsComponent.GetMaxVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();                   
                } break;
                case InputAction::ActionState::CONTINUE:
                case InputAction::ActionState::STOP: break;
            }
        }
        default: break;
    }
    
    return false;
}
