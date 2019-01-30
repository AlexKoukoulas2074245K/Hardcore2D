//
//  PlayerInputActionConsumer.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "PlayerInputActionConsumer.h"
#include "../ServiceLocator.h"
#include "../components/PhysicsComponent.h"
#include "../components/EntityComponentManager.h"
#include "../components/AnimationComponent.h"
#include "../commands/MoveEntityByCustomVelocityCommand.h"
#include "../commands/SetEntityCustomVelocityCommand.h"
#include "../events/EventCommunicator.h"
#include "../util/Logging.h"

#include <glm/glm.hpp>

PlayerInputActionConsumer::PlayerInputActionConsumer(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mServiceLocator(serviceLocator)
    , mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(entityId)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

PlayerInputActionConsumer::~PlayerInputActionConsumer()
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
                    MoveEntityByCustomVelocityCommand(mServiceLocator, mEntityId, glm::vec3(-entityPhysicsComponent.GetMaxVelocity().x, 0.0f, 0.0f)).Execute();
                    return true;
                } break;
                case InputAction::ActionState::STOP:
                {
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();
                    return true;
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
                    MoveEntityByCustomVelocityCommand(mServiceLocator, mEntityId, glm::vec3(entityPhysicsComponent.GetMaxVelocity().x, 0.0f, 0.0f)).Execute();
                    return true;
                } break;
                case InputAction::ActionState::STOP:
                {
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();
                    return true;
                } break;
            }
                        
            return true;
        } break;
        case InputAction::ActionType::JUMP:
        {
            switch (inputAction.mActionState)
            {
                case InputAction::ActionState::START:
                {   
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(entityPhysicsComponent.GetVelocity().x, entityPhysicsComponent.GetMaxVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();                   
                    return true;
                } break;
                case InputAction::ActionState::CONTINUE:
                case InputAction::ActionState::STOP: break;
            }            
        } break;
        case InputAction::ActionType::ATTACK:
        {
            switch (inputAction.mActionState)
            {
                case InputAction::ActionState::START:
                {
                    auto& playerAnimationComponent = mEntityComponentManager.GetComponent<AnimationComponent>(mEntityId);
                    playerAnimationComponent.PlayAnimationOnce(StringId("ground_melee"));
                    return true;
                } break;
                case InputAction::ActionState::CONTINUE:
                case InputAction::ActionState::STOP: break;
            }                         
        } break;
        default : break;
    }
    
    return false;
}
