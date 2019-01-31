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
#include "../commands/MoveEntityByCustomVelocityCommand.h"

const float BasicNinjaEnemyAIComponent::PLAYER_DETECTION_DISTANCE = 400.0f;
const float BasicNinjaEnemyAIComponent::PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION = 100.0f;

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
                mState = State::CHARGING;
            }
            else
            {
                const auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mThisEntityId);
                const auto& transformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mThisEntityId);

                MoveEntityByCustomVelocityCommand(mServiceLocator, mThisEntityId, glm::vec3(mMovingRight ? physicsComponent.GetMaxVelocity().x * 0.5f : physicsComponent.GetMinVelocity().x * 0.5f, 0.0f, 0.0f)).Execute();

                const auto distanceFromInitPosition = Abs(transformComponent.GetTranslation().x - mInitPosition.x);

                if (distanceFromInitPosition > PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION)
                {
                    mMovingRight = !mMovingRight;
                }
            }
        } break;

        case State::CHARGING:
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
