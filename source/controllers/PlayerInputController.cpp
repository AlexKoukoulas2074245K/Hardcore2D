//
//  PlayerInputController.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "PlayerInputController.h"
#include "../components/PhysicsComponent.h"
#include "../components/EntityComponentManager.h"
#include "../commands/SetEntityMovementVelocityCommand.h"
#include "../util/Logging.h"

#include <glm/glm.hpp>

static const float MOVE_X_VELOCITY = 240.0f;
static const float JUMP_HEIGHT = 80.0f;
static const float JUMP_LENGTH = 160.0f;

PlayerInputController::PlayerInputController(EntityComponentManager& entityComponentManager, const EntityId entityId)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
{
    
}

bool PlayerInputController::ConsumeInputAction(const InputHandler::InputAction& inputAction)
{
    switch (inputAction.first)
    {
        case InputHandler::ActionType::MOVE_LEFT:
        {
            auto& entityPhysicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);            
            
            switch (inputAction.second)
            {
                case InputHandler::ActionState::START:
				case InputHandler::ActionState::CONTINUE: if (entityPhysicsComponent.GetVelocity().x > -MOVE_X_VELOCITY) entityPhysicsComponent.GetVelocity().x -= MOVE_X_VELOCITY; break;
                case InputHandler::ActionState::STOP: entityPhysicsComponent.GetVelocity().x = 0.0f; break;
            }
                        
        } break;
        case InputHandler::ActionType::MOVE_RIGHT:
        {
            auto& entityPhysicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);            
            
            switch (inputAction.second)
            {
                case InputHandler::ActionState::START:
				case InputHandler::ActionState::CONTINUE: if (entityPhysicsComponent.GetVelocity().x < MOVE_X_VELOCITY) entityPhysicsComponent.GetVelocity().x += MOVE_X_VELOCITY; break;
				case InputHandler::ActionState::STOP: entityPhysicsComponent.GetVelocity().x = 0.0f; break;
            }
                        
        } break;
        case InputHandler::ActionType::JUMP:
        {
            auto& entityPhysicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);

            switch (inputAction.second)
            {
                case InputHandler::ActionState::START:
                {
                    //if (glm::abs(entityPhysicsComponent.GetVelocity().y) < 0.0001f)
                    //{
                        const auto targetYVelocity = (2.0f * JUMP_HEIGHT * MOVE_X_VELOCITY)/(0.5f * JUMP_LENGTH);
                        const auto targetYGravity = (-2.0f * JUMP_HEIGHT * (MOVE_X_VELOCITY*MOVE_X_VELOCITY))/((JUMP_LENGTH * 0.5f) * (JUMP_LENGTH * 0.5f));
                        entityPhysicsComponent.GetVelocity().y = targetYVelocity;
                        entityPhysicsComponent.GetGravity().y = targetYGravity;
                    //}
                } break;
                case InputHandler::ActionState::CONTINUE: break;
                case InputHandler::ActionState::STOP: break;
            }
        }
        case InputHandler::ActionType::ATTACK:
        {
            
        }
        case InputHandler::ActionType::NO_ACTION: break;
    }
    
    return false;
}
