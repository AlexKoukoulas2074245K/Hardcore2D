//
//  BasicNinjaEnemyAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 29/01/2019.
//

#include "BasicNinjaEnemyAIComponent.h"
#include "../util/Logging.h"
#include "../util/MathUtils.h"
#include "../ServiceLocator.h"
#include "../events/EventCommunicator.h"
#include "../events/AnnouncePlayerEntityIdEvent.h"
#include "../components/EntityComponentManager.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../commands/SetEntityCustomVelocityCommand.h"

const float BasicNinjaEnemyAIComponent::PLAYER_DETECTION_DISTANCE = 400.0f;
const float BasicNinjaEnemyAIComponent::PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION = 100.0f;
const float BasicNinjaEnemyAIComponent::PURSUING_MAX_DISTANCE_HORIZONTALLY = 500.0f;

BasicNinjaEnemyAIComponent::BasicNinjaEnemyAIComponent(const ServiceLocator& serviceLocator, const EntityId thisEntityId)
    : mServiceLocator(serviceLocator)
    , mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mThisEntityId(thisEntityId)
    , mState(State::INITIALIZE)
    , mMovingRight(true)    
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    mEventCommunicator->RegisterEventCallback<AnnouncePlayerEntityIdEvent>([this](const IEvent& event) 
    {
        mTargetEntityId = static_cast<const AnnouncePlayerEntityIdEvent&>(event).GetPlayerEntityId();
    });
}

BasicNinjaEnemyAIComponent::~BasicNinjaEnemyAIComponent()
{

}

void BasicNinjaEnemyAIComponent::VUpdate(const float)
{
    switch (mState)
    {
        case State::INITIALIZE:
        {
            mInitPosition = mEntityComponentManager.GetComponent<TransformComponent>(mThisEntityId).GetTranslation();
            mState = State::PATROLLING;
        } break;

        case State::PATROLLING: 
        {
            const auto distanceFromPlayer = Abs(mEntityComponentManager.GetComponent<TransformComponent>(mThisEntityId).GetTranslation().x -
                                            mEntityComponentManager.GetComponent<TransformComponent>(mTargetEntityId).GetTranslation().x);

            if (distanceFromPlayer < PLAYER_DETECTION_DISTANCE)
            {
                mState = State::LEAPING_TO_TARGET;
                const auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mThisEntityId);                
                
                SetEntityCustomVelocityCommand(mEntityComponentManager, mThisEntityId, glm::vec3(physicsComponent.GetMinVelocity().x,
                                                                                                 physicsComponent.GetMaxVelocity().y,
                                                                                                 physicsComponent.GetVelocity().z)).Execute();
            }
            else
            {
                const auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mThisEntityId);
                const auto& transformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mThisEntityId);
                
                const auto targetVelocity = glm::vec3(mMovingRight ? physicsComponent.GetMaxVelocity().x * 0.5f : physicsComponent.GetMinVelocity().x * 0.5f,
                                                      physicsComponent.GetVelocity().y,
                                                      physicsComponent.GetVelocity().z);

                SetEntityCustomVelocityCommand(mEntityComponentManager, mThisEntityId, targetVelocity).Execute();

                
                if (transformComponent.GetTranslation().x >= mInitPosition.x + PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION)
                {
                    mMovingRight = false;
                }
                else if (transformComponent.GetTranslation().x <= mInitPosition.x - PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION)
                {
                    mMovingRight = true;
                }
            }
        } break;

        case State::LEAPING_TO_TARGET:
        {
            const auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mThisEntityId);
            if (Abs(physicsComponent.GetVelocity().y) < 1.0f)
            {
                mState = State::PURSUING;
            }
        } break;

        case State::PURSUING:
        {

        } break;

        case State::DEAD:
        {

        } break;
    }
}

std::string BasicNinjaEnemyAIComponent::VSerializeToString() const
{
    return "";
}

bool BasicNinjaEnemyAIComponent::VInitializeFromString(const std::string&)
{
    return true;
}
