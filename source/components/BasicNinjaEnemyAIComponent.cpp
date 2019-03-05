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
#include "../commands/SetEntityVelocityAndAnimateCommand.h"
#include "../commands/SetEntityVelocityCommand.h"
#include "../commands/EntityMeleeAttackCommand.h"
#include "../commands/SetEntityFacingDirectionCommand.h"
#include "../events/EntityDamagedEvent.h"
#include "../events/EntityDestroyedEvent.h"
#include "../rendering/effects/EffectsManager.h"

const float BasicNinjaEnemyAIComponent::PLAYER_DETECTION_DISTANCE = 300.0f;
const float BasicNinjaEnemyAIComponent::PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION = 100.0f;
const float BasicNinjaEnemyAIComponent::PURSUING_MELEE_ATTACK_DISTANCE = 100.0f;
const float BasicNinjaEnemyAIComponent::MELEE_ATTACK_COOLDOWN = 2.0f;

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
    auto& physicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mThisEntityId);
    const auto& targetTransformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mTargetEntityId);
    const auto& transformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mThisEntityId);       
    const auto currentFacingDirection = mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).GetCurrentFacingDirection();

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
                mState = State::PURSUING;
            }
            else
            {                
                SetEntityVelocityAndAnimateCommand(mEntityComponentManager, mThisEntityId, glm::vec3(physicsComponent.GetVelocity().x + (mMovingRight ? physicsComponent.GetMaxVelocity().x : physicsComponent.GetMinVelocity().x), 0.0f, 0.0f)).VExecute();                             
                
                if (transformComponent.GetTranslation().x >= mInitPosition.x + PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION)
                {
                    mMovingRight = false;
                }
                else if (transformComponent.GetTranslation().x <= mInitPosition.x - PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION)
                {
                    mMovingRight = true;
                }                
                
                physicsComponent.SetPushbackForce(glm::vec3(mDamagePushback, 0.0f, 0.0f));
            }
        } break;

        case State::PURSUING:
        {            
            const auto distanceFromPlayer = Abs(transformComponent.GetTranslation().x - targetTransformComponent.GetTranslation().x);
            physicsComponent.SetMaxVelocity(glm::vec3(200.0f, 720.0f, 0.0f));
            physicsComponent.SetMinVelocity(glm::vec3(-200.0f, -720.0f, 0.0f));

            if (distanceFromPlayer < PURSUING_MELEE_ATTACK_DISTANCE)
            {                
                
                SetEntityVelocityAndAnimateCommand(mEntityComponentManager, mThisEntityId, glm::vec3(0.0f, physicsComponent.GetVelocity().y, physicsComponent.GetVelocity().z)).VExecute();                                                   
                mTimer -= dt;
                if (mTimer <= 0.0f)
                {
                    mTimer = MELEE_ATTACK_COOLDOWN;
                    EntityMeleeAttackCommand(mServiceLocator, mThisEntityId).VExecute();
                }
            }
            else
            {
                const auto targetXVelocity = targetTransformComponent.GetTranslation().x > transformComponent.GetTranslation().x ? physicsComponent.GetMaxVelocity().x * 1.5f : physicsComponent.GetMinVelocity().x * 1.5f;
                SetEntityVelocityAndAnimateCommand(mEntityComponentManager, mThisEntityId, glm::vec3(targetXVelocity, physicsComponent.GetVelocity().y, physicsComponent.GetVelocity().z)).VExecute();
            }
            
        } break;
            
        case State::DEAD:
        {
            SetEntityVelocityCommand(mEntityComponentManager, mThisEntityId, glm::vec3(0.0f, 0.0f, 0.0f)).VExecute();
        } break;
    }


    if (mDamagePushback > 0.1f)
    {
        mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).SetFacingDirection(currentFacingDirection);
        mDamagePushback = 0.0f;
    }    
}

void BasicNinjaEnemyAIComponent::OnAnnouncePlayerEntityId(const IEvent& event)
{
    mTargetEntityId = static_cast<const AnnouncePlayerEntityIdEvent&>(event).GetPlayerEntityId();
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

    const auto& animationComponent = mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId);
    if (animationComponent.GetCurrentFacingDirection() == FacingDirection::LEFT)
    {
        mDamagePushback = 10000.0f;
    }
    else
    {
        mDamagePushback = 2000.0f;
    }
    mState = State::PURSUING;

    if (actualEvent.GetHealthRemaining() > 0.0f)
    {
        mEffectsManager.PlayEffect(mEntityComponentManager.GetComponent<TransformComponent>(actualEvent.GetDamageSenderEntityId()).GetTranslation(), EffectsManager::EffectType::BLOOD_SPURT);
        return;
    }

    mState = State::DEAD;
    
    SetEntityVelocityAndAnimateCommand(mEntityComponentManager, mThisEntityId, glm::vec3(0.0f, 0.0f, 0.0f)).VExecute();
    mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).PlayAnimation(StringId("death"), false, false, AnimationComponent::AnimationPriority::HIGH, [this]() 
    {        
        mEntityComponentManager.RemoveComponent<DamageComponent>(mThisEntityId);
        mEntityComponentManager.RemoveComponent<HealthComponent>(mThisEntityId);
    });
}

void BasicNinjaEnemyAIComponent::OnLeapingComplete()
{
    mState = State::PURSUING;
    SetEntityVelocityAndAnimateCommand(mEntityComponentManager, mThisEntityId, glm::vec3(0.0f, 0.0f, 0.0f)).VExecute();
    EntityMeleeAttackCommand(mServiceLocator, mThisEntityId).VExecute();
    mTimer = MELEE_ATTACK_COOLDOWN;
}
