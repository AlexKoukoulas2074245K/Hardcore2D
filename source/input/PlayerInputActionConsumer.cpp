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

static const float HORIZONTAL_SPEED = 240.0f;
static const float JUMP_START_SPEED = 480.0f;

PlayerInputActionConsumer::PlayerInputActionConsumer(EntityComponentManager& entityComponentManager, const EntityId entityId)
    : mEntityComponentManager(entityComponentManager)
    , mEntityId(entityId)
{
    
}

bool PlayerInputActionConsumer::VConsumeInputAction(const InputAction& inputAction) const
{
    auto& entityPhysicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mEntityId);
    switch (inputAction.first)
    {
        case ActionType::MOVE_LEFT:
        {
            switch (inputAction.second)
            {
                case ActionState::START:
				case ActionState::CONTINUE:
                {
                    MoveEntityByCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(-HORIZONTAL_SPEED, 0.0f, 0.0f)).Execute();
                } break;
                case ActionState::STOP:
                {
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();
                } break;
            }
                        
        } break;
        case ActionType::MOVE_RIGHT:
        {
            switch (inputAction.second)
            {
                case ActionState::START:
                case ActionState::CONTINUE:
                {
                    MoveEntityByCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(HORIZONTAL_SPEED, 0.0f, 0.0f)).Execute();
                } break;
                case ActionState::STOP:
                {
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();
                } break;
            }
                        
        } break;
        case ActionType::JUMP:
        {
            switch (inputAction.second)
            {
                case ActionState::START:
                {   
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(entityPhysicsComponent.GetVelocity().x, JUMP_START_SPEED, entityPhysicsComponent.GetVelocity().z)).Execute();                   
                } break;
                case ActionState::CONTINUE:
                case ActionState::STOP: break;
            }
        }
        case ActionType::ATTACK:
        {
            
        }
        case ActionType::NO_ACTION: break;
    }
    
    return false;
}
