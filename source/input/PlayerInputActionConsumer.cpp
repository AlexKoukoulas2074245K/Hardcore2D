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
#include "../commands/SetEntityCustomVelocityCommand.h"
#include "../commands/EntityMeleeAttackCommand.h"
#include "../events/EventCommunicator.h"
#include "../util/Logging.h"
#include "../events/PlayerChangedDirectionEvent.h"

#include <glm/glm.hpp>

PlayerInputActionConsumer::PlayerInputActionConsumer(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mServiceLocator(serviceLocator)
    , mEntityComponentManager(mServiceLocator.ResolveService<EntityComponentManager>())
    , mEntityId(entityId)
    , mEventCommunicator(mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
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
                {
                    mEventCommunicator->DispatchEvent(std::make_unique<PlayerChangedDirectionEvent>(FacingDirection::LEFT));
                } break;
				case InputAction::ActionState::CONTINUE:
                {
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(-entityPhysicsComponent.GetMaxVelocity().x, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();
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
                {
                    mEventCommunicator->DispatchEvent(std::make_unique<PlayerChangedDirectionEvent>(FacingDirection::RIGHT));
                } break;
                case InputAction::ActionState::CONTINUE:
                {
                    SetEntityCustomVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(entityPhysicsComponent.GetMaxVelocity().x, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).Execute();
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
                    EntityMeleeAttackCommand(mServiceLocator, mEntityId).Execute();
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
