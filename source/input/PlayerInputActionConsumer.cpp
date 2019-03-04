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
#include "../commands/SetEntityVelocityCommand.h"
#include "../commands/SetEntityVelocityAndAnimateCommand.h"
#include "../commands/EntityMeleeAttackCommand.h"
#include "../commands/EntityRangedShurikenAttackCommand.h"
#include "../events/EventCommunicator.h"
#include "../events/PlayerMeleeAttackEvent.h"
#include "../events/PlayerRangedAttackEvent.h"
#include "../events/PlayerRespawnEvent.h"
#include "../events/PlayerChangedDirectionEvent.h"
#include "../events/PlayerJumpEvent.h"
#include "../events/PlayerKilledEvent.h"
#include "../game/PlayerBehaviorController.h"
#include "../util/Logging.h"

#include <glm/glm.hpp>

PlayerInputActionConsumer::PlayerInputActionConsumer(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mServiceLocator(serviceLocator)
    , mPlayerBehaviorController(mServiceLocator.ResolveService<PlayerBehaviorController>())
    , mEntityComponentManager(mServiceLocator.ResolveService<EntityComponentManager>())
    , mEventCommunicator(mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
    , mEntityId(entityId)
    , mPlayerKilled(false)
{
    RegisterEventCallbacks();
}

PlayerInputActionConsumer::~PlayerInputActionConsumer()
{
}

bool PlayerInputActionConsumer::VConsumeInputAction(const InputAction& inputAction) const
{
    if (inputAction.mActionType == InputAction::ActionType::RESPAWN &&
        inputAction.mActionState == InputAction::ActionState::START)
    {
        mEventCommunicator->DispatchEvent(std::make_unique<PlayerRespawnEvent>());
        return true;
    }
    
    if (mPlayerKilled)
    {
        SetEntityVelocityCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, 0.0f, 0.0f)).VExecute();
        return false;
    }
    
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
                    SetEntityVelocityAndAnimateCommand(mEntityComponentManager, mEntityId, glm::vec3(-entityPhysicsComponent.GetMaxVelocity().x, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).VExecute();
                    return true;
                } break;
                case InputAction::ActionState::STOP:
                {
                    SetEntityVelocityAndAnimateCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).VExecute();
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
                    SetEntityVelocityAndAnimateCommand(mEntityComponentManager, mEntityId, glm::vec3(entityPhysicsComponent.GetMaxVelocity().x, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).VExecute();
                    return true;
                } break;
                case InputAction::ActionState::STOP:
                {
                    SetEntityVelocityAndAnimateCommand(mEntityComponentManager, mEntityId, glm::vec3(0.0f, entityPhysicsComponent.GetVelocity().y, entityPhysicsComponent.GetVelocity().z)).VExecute();
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
                    if (mPlayerBehaviorController.CanJump())
                    {
                        SetEntityVelocityAndAnimateCommand(mEntityComponentManager, mEntityId, glm::vec3(entityPhysicsComponent.GetVelocity().x, entityPhysicsComponent.GetMaxVelocity().y, entityPhysicsComponent.GetVelocity().z)).VExecute();
                        mEventCommunicator->DispatchEvent(std::make_unique<PlayerJumpEvent>());
                    }
                    
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
                    if (mPlayerBehaviorController.IsMeleeAttackAvailable())
                    {
                        EntityMeleeAttackCommand(mServiceLocator, mEntityId).VExecute();
                        mEventCommunicator->DispatchEvent(std::make_unique<PlayerMeleeAttackEvent>());
                    }
                    return true;
                } break;
                case InputAction::ActionState::CONTINUE:
                case InputAction::ActionState::STOP: break;
            }                         
        } break;
        case InputAction::ActionType::RANGED:
        {
            switch (inputAction.mActionState)
            {
                case InputAction::ActionState::START:
                {
                    if (mPlayerBehaviorController.IsRangedAttackAvailable())
                    {
                        EntityRangedShurikenAttackCommand(mServiceLocator, mEntityId).VExecute();
                        mEventCommunicator->DispatchEvent(std::make_unique<PlayerRangedAttackEvent>());
                    }
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

void PlayerInputActionConsumer::RegisterEventCallbacks()
{
    mEventCommunicator->RegisterEventCallback<PlayerKilledEvent>([this](const IEvent&)
    {
        mPlayerKilled = true;
    });
}
