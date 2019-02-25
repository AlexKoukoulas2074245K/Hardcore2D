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
#include "../components/DamageComponent.h"
#include "../components/EntityComponentManager.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/HealthComponent.h"
#include "../commands/SetEntityCustomVelocityCommand.h"
#include "../commands/EntityMeleeAttackCommand.h"
#include "../commands/SetEntityFacingDirectionCommand.h"
#include "../events/EntityDamagedEvent.h"
#include "../events/EntityDestroyedEvent.h"
#include "../rendering/effects/EffectsManager.h"

const float BasicNinjaEnemyAIComponent::PLAYER_DETECTION_DISTANCE = 300.0f;
const float BasicNinjaEnemyAIComponent::PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION = 100.0f;
const float BasicNinjaEnemyAIComponent::PURSUING_MELEE_ATTACK_DISTANCE = 100.0f;

BasicNinjaEnemyAIComponent::BasicNinjaEnemyAIComponent(const ServiceLocator& serviceLocator, const EntityId thisEntityId)
    : mServiceLocator(serviceLocator)
    , mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mEffectsManager(serviceLocator.ResolveService<EffectsManager>())
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
    , mThisEntityId(thisEntityId)
    , mState(State::INITIALIZE)
    , mMovingRight(true)
    , mTimer(RandomFloat(0.0f, 1.0f))
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
            mTimer -= dt;
            if (mTimer <= 0.0f)
            {
                mInitPosition = mEntityComponentManager.GetComponent<TransformComponent>(mThisEntityId).GetTranslation();
                mState = State::PATROLLING;
                mTimer = 0.0f;
            }
            
        } break;

        case State::PATROLLING: 
        {
            const auto distanceFromPlayer = Abs(transformComponent.GetTranslation().x - targetTransformComponent.GetTranslation().x);

            if (distanceFromPlayer < PLAYER_DETECTION_DISTANCE)
            {
                mState = State::LEAPING_TO_TARGET;
                SetEntityFacingDirectionCommand(mEntityComponentManager, mThisEntityId, transformComponent.GetTranslation().x > targetTransformComponent.GetTranslation().x ? FacingDirection::RIGHT : FacingDirection::LEFT).VExecute();
                mMovingRight = animationComponent.GetCurrentFacingDirection() == FacingDirection::RIGHT;
                
                SetEntityCustomVelocityCommand(mEntityComponentManager, mThisEntityId, glm::vec3(mMovingRight ? physicsComponent.GetMinVelocity().x * 0.25f : physicsComponent.GetMaxVelocity().x * 0.25f,
                                                                                                 physicsComponent.GetMaxVelocity().y * 0.8f,
                                                                                                 physicsComponent.GetVelocity().z)).VExecute();
                
            }
            else
            {
                const auto targetVelocity = glm::vec3(mMovingRight ? physicsComponent.GetMaxVelocity().x * 0.07f : physicsComponent.GetMinVelocity().x * 0.07f,
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

        case State::PURSUING:
        {
            const auto& targetTransformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mTargetEntityId);
            const auto distanceFromPlayer = Abs(transformComponent.GetTranslation().x - targetTransformComponent.GetTranslation().x);
            
            if (distanceFromPlayer < PURSUING_MELEE_ATTACK_DISTANCE)
            {
                mTimer -= dt;
                if (mTimer <= 0.0f)
                {
                    mTimer = 2.0f;
                    EntityMeleeAttackCommand(mServiceLocator, mThisEntityId).VExecute();
                }
            }
            else
            {
                const auto targetXVelocity = targetTransformComponent.GetTranslation().x > transformComponent.GetTranslation().x ? physicsComponent.GetMaxVelocity().x * 0.13f : physicsComponent.GetMinVelocity().x * 0.13f;
                SetEntityCustomVelocityCommand(mEntityComponentManager, mThisEntityId, glm::vec3(targetXVelocity, 0.0f, 0.0f)).VExecute();
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
        if (mThisEntityId == collidedEntitiesPair.first && mEntityComponentManager.GetComponent<PhysicsComponent>(collidedEntitiesPair.second).GetBodyType() == PhysicsComponent::BodyType::STATIC)
        {
            OnLeapingComplete();
        }
    }
}

void BasicNinjaEnemyAIComponent::OnEntityDamagedEvent(const IEvent& event)
{    
    if (mState == State::DEAD)
    {
        return;
    }

    const auto& actualEvent = static_cast<const EntityDamagedEvent&>(event);

    if (actualEvent.GetDamagedEntityId() != mThisEntityId)
    {
        return;
    }

    if (actualEvent.GetHealthRemaining() > 0.0f)
    {
        mEffectsManager.PlayEffect(mEntityComponentManager.GetComponent<TransformComponent>(actualEvent.GetDamageSenderEntityId()).GetTranslation(), EffectsManager::EffectType::BLOOD_SPURT);
        return;
    }

    mState = State::DEAD;
    
    mEntityComponentManager.GetComponent<PhysicsComponent>(mThisEntityId).GetVelocity() = glm::vec3(0.0f, 0.0f, 0.0f);
    mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).PlayAnimationOnce(StringId("death"), true, [this]() 
    {
        mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).SetPause(true);
        mEntityComponentManager.RemoveComponent<DamageComponent>(mThisEntityId);
        mEntityComponentManager.RemoveComponent<HealthComponent>(mThisEntityId);
    });
}

void BasicNinjaEnemyAIComponent::OnLeapingComplete()
{
    mState = State::PURSUING;
    SetEntityCustomVelocityCommand(mEntityComponentManager, mThisEntityId, glm::vec3(0.0f, 0.0f, 0.0f)).VExecute();
    EntityMeleeAttackCommand(mServiceLocator, mThisEntityId).VExecute();
    mTimer = 0.5f;
}
