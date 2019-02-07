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
#include "../events/EntityCollisionEvent.h"
#include "../components/AnimationComponent.h"
#include "../components/EntityComponentManager.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../commands/SetEntityCustomVelocityCommand.h"
#include "../commands/EntityMeleeAttackCommand.h"
#include "../commands/SetEntityFacingDirectionCommand.h"
#include "../events/EntityDamagedEvent.h"
#include "../events/EntityDestroyedEvent.h"

const float BasicNinjaEnemyAIComponent::PLAYER_DETECTION_DISTANCE = 400.0f;
const float BasicNinjaEnemyAIComponent::PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION = 100.0f;
const float BasicNinjaEnemyAIComponent::PURSUING_MAX_DISTANCE_HORIZONTALLY = 500.0f;
const float BasicNinjaEnemyAIComponent::FOCUSING_TIMER = 3.0f;
const float BasicNinjaEnemyAIComponent::CHARGING_TIMER = 1.5f;

BasicNinjaEnemyAIComponent::BasicNinjaEnemyAIComponent(const ServiceLocator& serviceLocator, const EntityId thisEntityId)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mThisEntityId(thisEntityId)
    , mState(State::INITIALIZE)
    , mMovingRight(true)
    , mTimer(0.0f)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    mEventCommunicator->RegisterEventCallback<AnnouncePlayerEntityIdEvent>([this](const IEvent& event) 
    {
        OnAnnouncePlayerEntityId(event);
    });
    mEventCommunicator->RegisterEventCallback<EntityCollisionEvent>([this](const IEvent& event)
    {
        OnEntityCollisionEvent(event);
    });
    mEventCommunicator->RegisterEventCallback<EntityDamagedEvent>([this](const IEvent& event)
    {
        OnEntityDamagedEvent(event);
    });
}

BasicNinjaEnemyAIComponent::~BasicNinjaEnemyAIComponent()
{

}

void BasicNinjaEnemyAIComponent::VUpdate(const float dt)
{
    const auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mThisEntityId);
    const auto& targetTransformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mTargetEntityId);
    const auto& transformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mThisEntityId);
    const auto& animationComponent = mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId);
    
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
                SetEntityFacingDirectionCommand(mEntityComponentManager, mThisEntityId, transformComponent.GetTranslation().x > targetTransformComponent.GetTranslation().x ? FacingDirection::RIGHT : FacingDirection::LEFT).VExecute();
                mMovingRight = animationComponent.GetCurrentFacingDirection() == FacingDirection::RIGHT;
                
                SetEntityCustomVelocityCommand(mEntityComponentManager, mThisEntityId, glm::vec3(mMovingRight ? physicsComponent.GetMinVelocity().x * 0.25f : physicsComponent.GetMaxVelocity().x * 0.25f,
                                                                                                 physicsComponent.GetMaxVelocity().y,
                                                                                                 physicsComponent.GetVelocity().z)).VExecute();
                
            }
            else
            {
                const auto targetVelocity = glm::vec3(mMovingRight ? physicsComponent.GetMaxVelocity().x * 0.125f : physicsComponent.GetMinVelocity().x * 0.125f,
                                                      physicsComponent.GetVelocity().y,
                                                      physicsComponent.GetVelocity().z);

                SetEntityCustomVelocityCommand(mEntityComponentManager, mThisEntityId, targetVelocity).VExecute();
                
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
        } break;

        case State::FOCUSING:
        {
            SetEntityFacingDirectionCommand(mEntityComponentManager, mThisEntityId, transformComponent.GetTranslation().x < targetTransformComponent.GetTranslation().x ? FacingDirection::RIGHT : FacingDirection::LEFT).VExecute();
            mMovingRight = animationComponent.GetCurrentFacingDirection() == FacingDirection::RIGHT;
            
            mTimer -= dt;
            if (mTimer <= 0.0f)
            {
                mTimer = CHARGING_TIMER;
                mState = State::CHARGING_PLAYER;
            }
        } break;

        case State::CHARGING_PLAYER:
        {
            const auto chargingSpeed = mMovingRight ? physicsComponent.GetMaxVelocity().x * (mTimer * 0.5f) : physicsComponent.GetMinVelocity().x * (mTimer * 0.5f);
            SetEntityCustomVelocityCommand(mEntityComponentManager, mThisEntityId, glm::vec3(chargingSpeed, physicsComponent.GetVelocity().y, physicsComponent.GetVelocity().z)).VExecute();
            
            mTimer -= dt;
            if (mTimer <= 0.0f)
            {
                mState = State::LEAPING_TO_TARGET;
                SetEntityFacingDirectionCommand(mEntityComponentManager, mThisEntityId, transformComponent.GetTranslation().x > targetTransformComponent.GetTranslation().x ? FacingDirection::RIGHT : FacingDirection::LEFT).VExecute();
                mMovingRight = animationComponent.GetCurrentFacingDirection() == FacingDirection::RIGHT;
                
                SetEntityCustomVelocityCommand(mEntityComponentManager, mThisEntityId, glm::vec3(mMovingRight ? physicsComponent.GetMinVelocity().x * 0.25f : physicsComponent.GetMaxVelocity().x * 0.25f,
                                                                                                 physicsComponent.GetMaxVelocity().y,
                                                                                                 physicsComponent.GetVelocity().z)).VExecute();
                
            }
        } break;
            
        case State::DEAD:
        {

        } break;
    }
}

void BasicNinjaEnemyAIComponent::OnAnnouncePlayerEntityId(const IEvent& event)
{
    mTargetEntityId = static_cast<const AnnouncePlayerEntityIdEvent&>(event).GetPlayerEntityId();
}

void BasicNinjaEnemyAIComponent::OnEntityCollisionEvent(const IEvent& event)
{
    if (mState == State::LEAPING_TO_TARGET)
    {
        const auto& collidedEntitiesPair = static_cast<const EntityCollisionEvent&>(event).GetCollidedEntityIds();
        if (mThisEntityId == collidedEntitiesPair.first)
        {
            const auto& otherEntityPhysicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(collidedEntitiesPair.second);
            OnLeapingComplete(otherEntityPhysicsComponent);
        }
        else if (mThisEntityId == collidedEntitiesPair.second)
        {
            const auto& otherEntityPhysicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(collidedEntitiesPair.first);
            OnLeapingComplete(otherEntityPhysicsComponent);
        }
    }
}

void BasicNinjaEnemyAIComponent::OnEntityDamagedEvent(const IEvent& event)
{
    const auto& actualEvent = static_cast<const EntityDamagedEvent&>(event);

    if (mState == State::DEAD)
    {
        return;
    }

    if (actualEvent.GetDamagedEntityId() != mThisEntityId)
    {
        return;
    }

    if (actualEvent.GetHealthRemaining() > 0.0f)
    {
        return;
    }

    mState = State::DEAD;
    mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).PlayAnimation(StringId("death"), [this]() 
    {
        mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).SetPause(true);        
    });
}

void BasicNinjaEnemyAIComponent::OnLeapingComplete(const PhysicsComponent& otherEntityPhysicsComponent)
{
    if (otherEntityPhysicsComponent.GetBodyType() == PhysicsComponent::BodyType::STATIC)
    {
        mTimer = FOCUSING_TIMER;
        mState = State::FOCUSING;
        SetEntityCustomVelocityCommand(mEntityComponentManager, mThisEntityId, glm::vec3(0.0f, 0.0f, 0.0f)).VExecute();
    }
}
