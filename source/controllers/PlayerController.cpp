//
//  PlayerController.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "PlayerController.h"
#include "../components/PhysicsComponent.h"
#include "../components/EntityComponentManager.h"
#include "../commands/MoveEntityByCustomVelocityCommand.h"
#include "../commands/SetEntityCustomVelocityCommand.h"
#include "../util/Logging.h"

#include <glm/glm.hpp>

static const float MOVE_X_VELOCITY = 240.0f;
static const float JUMP_HEIGHT = 80.0f;
static const float JUMP_LENGTH = 160.0f;

PlayerController::PlayerController(EntityComponentManager& entityComponentManager, const EntityId entityId)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
{
    
}

bool PlayerController::ConsumeInputAction(const InputHandler::InputAction& inputAction)
{
    auto& entityPhysicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);
    switch (inputAction.first)
    {
        case InputHandler::ActionType::MOVE_LEFT:
        {
            switch (inputAction.second)
            {
                case InputHandler::ActionState::START:
				case InputHandler::ActionState::CONTINUE:
                {
                    MoveEntityByCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(-MOVE_X_VELOCITY, 0.0f, 0.0f)).Execute();
                } break;
                case InputHandler::ActionState::STOP:
                {
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();
                } break;
            }
                        
        } break;
        case InputHandler::ActionType::MOVE_RIGHT:
        {
            switch (inputAction.second)
            {
                case InputHandler::ActionState::START:
                case InputHandler::ActionState::CONTINUE:
                {
                    MoveEntityByCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(MOVE_X_VELOCITY, 0.0f, 0.0f)).Execute();
                } break;
                case InputHandler::ActionState::STOP:
                {
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();
                } break;
            }
                        
        } break;
        case InputHandler::ActionType::JUMP:
        {
            switch (inputAction.second)
            {
                case InputHandler::ActionState::START:
                {
                    const auto targetJumpVelocity = (2.0f * JUMP_HEIGHT * MOVE_X_VELOCITY)/(0.5f * JUMP_LENGTH);
                    MoveEntityByCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, targetJumpVelocity, 0.0f)).Execute();
                } break;
                case InputHandler::ActionState::CONTINUE:
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
