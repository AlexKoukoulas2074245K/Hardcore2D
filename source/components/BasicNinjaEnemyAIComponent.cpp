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
#include "../commands/SetVelocityAndAnimateCommand.h"
#include "../commands/SetVelocityCommand.h"
#include "../commands/EntityMeleeAttackCommand.h"
#include "../commands/SetFacingDirectionCommand.h"
#include "../events/EntityDamagedEvent.h"
#include "../events/EntityDestroyedEvent.h"
#include "../game/Level.h"
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
    , mInitializationTimer(RandomFloat(0.0f, 1.0f), [this](){ OnInitializationTimerTick(); })
    , mAttackTimer(MELEE_ATTACK_COOLDOWN, [this](){ OnAttackTimerTick(); })
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
    
    switch (mState)
    {
        case State::INITIALIZE:
        {
            mInitializationTimer.Update(dt);
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
                SetVelocityAndAnimateCommand(mEntityComponentManager, mThisEntityId, glm::vec3(physicsComponent.GetVelocity().x + (mMovingRight ? physicsComponent.GetMaxVelocity().x : physicsComponent.GetMinVelocity().x), physicsComponent.GetVelocity().y, physicsComponent.GetVelocity().z)).VExecute();                             
                
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

        case State::PURSUING:
        {            
            const auto distanceFromPlayer = Abs(transformComponent.GetTranslation().x - targetTransformComponent.GetTranslation().x);
            
            // Increase velocity caps
            physicsComponent.SetMaxVelocity(glm::vec3(200.0f, 720.0f, 0.0f));
            physicsComponent.SetMinVelocity(glm::vec3(-200.0f, -720.0f, 0.0f));                        

            if (distanceFromPlayer < PURSUING_MELEE_ATTACK_DISTANCE)
            {                          
                SetVelocityAndAnimateCommand(mEntityComponentManager, mThisEntityId, glm::vec3(0.0f, physicsComponent.GetVelocity().y, physicsComponent.GetVelocity().z)).VExecute();
                mAttackTimer.Update(dt);
            }
            else
            {
                const auto targetXVelocity = targetTransformComponent.GetTranslation().x > transformComponent.GetTranslation().x ? physicsComponent.GetMaxVelocity().x * 0.7f : physicsComponent.GetMinVelocity().x * 0.7f;
                SetVelocityAndAnimateCommand(mEntityComponentManager, mThisEntityId, glm::vec3(physicsComponent.GetVelocity().x + targetXVelocity, physicsComponent.GetVelocity().y, physicsComponent.GetVelocity().z)).VExecute();
            }
            
            const auto isOtherEntityToTheLeftOfThisOne = targetTransformComponent.GetTranslation().x < transformComponent.GetTranslation().x;
            SetFacingDirectionCommand(mEntityComponentManager, mThisEntityId, isOtherEntityToTheLeftOfThisOne ? FacingDirection::LEFT : FacingDirection::RIGHT).VExecute();

        } break;
            
        case State::DEAD:
        {
            SetVelocityCommand(mEntityComponentManager, mThisEntityId, glm::vec3(0.0f, 0.0f, 0.0f)).VExecute();
        } break;
    }
}

void BasicNinjaEnemyAIComponent::OnInitializationTimerTick()
{
    mInitPosition = mEntityComponentManager.GetComponent<TransformComponent>(mThisEntityId).GetTranslation();
    mState = State::PATROLLING;
}

void BasicNinjaEnemyAIComponent::OnAttackTimerTick()
{
    EntityMeleeAttackCommand(mServiceLocator, mThisEntityId).VExecute();
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
    
    auto& thisPhysicsComponent = mEntityComponentManager.GetComponent<PhysicsComponent>(mThisEntityId);    
    const auto& animationComponent = mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId);

    if (animationComponent.GetCurrentFacingDirection() == FacingDirection::LEFT)
    {
        thisPhysicsComponent.SetPushbackForce(glm::vec3(1000.0f, 0.0f, 0.0f));
    }    
    else
    {
        thisPhysicsComponent.SetPushbackForce(glm::vec3(-1000.0f, 0.0f, 0.0f));
    }
    
    mState = State::PURSUING;

    if (actualEvent.GetHealthRemaining() > 0.0f)
    {
        mEffectsManager.PlayEffect(mEntityComponentManager.GetComponent<TransformComponent>(actualEvent.GetDamageSenderEntityId()).GetTranslation(), EffectsManager::EffectType::BLOOD_SPURT_MULTI);
        return;
    }

    mState = State::DEAD;
    
    SetVelocityAndAnimateCommand(mEntityComponentManager, mThisEntityId, glm::vec3(0.0f, 0.0f, 0.0f)).VExecute();
    
    const auto& level = mServiceLocator.ResolveService<Level>();
    
    const auto diedFromFlameBreath = level.GetEntityNameFromId(actualEvent.GetDamageSenderEntityId()).GetString() == "flame_breath";
    mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).PlayAnimation(StringId(diedFromFlameBreath ? "flame_death" : "death"), false, false, AnimationComponent::AnimationPriority::HIGH, [this]()
    {        
        mEntityComponentManager.RemoveComponent<DamageComponent>(mThisEntityId);
        mEntityComponentManager.RemoveComponent<HealthComponent>(mThisEntityId);
    });
}
