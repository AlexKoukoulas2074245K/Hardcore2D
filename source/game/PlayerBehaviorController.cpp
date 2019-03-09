//
//  PlayerBehaviorController.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 06/02/2019.
//

#include "PlayerBehaviorController.h"
#include "../ServiceLocator.h"
#include "../events/EventCommunicator.h"
#include "../events/PlayerMeleeAttackEvent.h"
#include "../events/EntityDamagedEvent.h"
#include "../events/AnnouncePlayerEntityIdEvent.h"
#include "../events/PlayerJumpEvent.h"
#include "../events/EntityCollisionEvent.h"
#include "../events/PlayerKilledEvent.h"
#include "../events/PlayerFlameBreathAttackEvent.h"
#include "../commands/SetVelocityAndAnimateCommand.h"
#include "../components/AnimationComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/HealthComponent.h"
#include "../components/EntityComponentManager.h"
#include "../components/TransformComponent.h"
#include "../rendering/effects/EffectsManager.h"
#include "../util/MathUtils.h"
#include "../util/Logging.h"

const float PlayerBehaviorController::DEFAULT_PLAYER_MELEE_ATTACK_RECHARGE_DURATION = 0.5f;
const float PlayerBehaviorController::DEFAULT_PLAYER_FLAME_BREATH_ATTACK_COOLDOWN_DURATION = 1.0f;
const float PlayerBehaviorController::BLOOD_DROP_DEATH_ANIMATION_COOLDOWN = 0.08f;
const int PlayerBehaviorController::DEFAULT_JUMP_COUNT = 2;

PlayerBehaviorController::PlayerBehaviorController(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
    , mEntityComponentManager(nullptr)
    , mEventCommunicator(nullptr)
    , mPlayerEntityId(-1)
    , mJumpCount(DEFAULT_JUMP_COUNT)
    , mJumpsAvailable(mJumpCount)
    , mIsMeleeAttackReady(true)
    , mIsFlameBreathAttackReady(true)
    , mPlayerKilled(false)
    , mMeleeAttackCooldownTimer(DEFAULT_PLAYER_MELEE_ATTACK_RECHARGE_DURATION, [this](){ OnMeleeAttackCooldownTimerTick(); })
    , mFlameBreathCooldownTimer(DEFAULT_PLAYER_FLAME_BREATH_ATTACK_COOLDOWN_DURATION, [this](){ OnFlameBreathCooldownTimerTick(); })
    , mBloodDropAnimationTimer(BLOOD_DROP_DEATH_ANIMATION_COOLDOWN, [this](){ OnBloodDropAnimationTimerTick(); })
{
    
}

bool PlayerBehaviorController::VInitialize()
{
    mEventCommunicator = mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
    mEffectsManager = &(mServiceLocator.ResolveService<EffectsManager>());
    
    RegisterEventCallbacks();
    return true;
}

void PlayerBehaviorController::Update(const float dt)
{
    mMeleeAttackCooldownTimer.Update(dt);
    mFlameBreathCooldownTimer.Update(dt);
    mBloodDropAnimationTimer.Update(dt);
    
    if (mJumpsAvailable < mJumpCount)
    {
        mEntityComponentManager->GetComponent<AnimationComponent>(mPlayerEntityId).PlayAnimation(StringId("jumping"), false, false);
    }
}

bool PlayerBehaviorController::CanJump() const
{
    return mJumpsAvailable > 0;
}

bool PlayerBehaviorController::IsMeleeAttackAvailable() const
{
    return mIsMeleeAttackReady;
}

bool PlayerBehaviorController::IsFlameBreathAttackAvailable() const
{
    return mIsFlameBreathAttackReady;
}

void PlayerBehaviorController::RegisterEventCallbacks()
{
    mEventCommunicator->RegisterEventCallback<AnnouncePlayerEntityIdEvent>([this](const IEvent& event)
    {
        mPlayerEntityId = static_cast<const AnnouncePlayerEntityIdEvent&>(event).GetPlayerEntityId();
        mPlayerKilled = false;
    });

    mEventCommunicator->RegisterEventCallback<PlayerMeleeAttackEvent>([this](const IEvent&)
    {
        mIsMeleeAttackReady = false;
        mMeleeAttackCooldownTimer.Reset();
    });
    
    mEventCommunicator->RegisterEventCallback<PlayerFlameBreathAttackEvent>([this](const IEvent&)
    {
        mIsFlameBreathAttackReady = false;
        mFlameBreathCooldownTimer.Reset();
    });
    
    mEventCommunicator->RegisterEventCallback<PlayerJumpEvent>([this](const IEvent&)
    {
        if (--mJumpsAvailable < 0)
        {
            mJumpsAvailable = 0;
        }
    });

    mEventCommunicator->RegisterEventCallback<EntityCollisionEvent>([this](const IEvent& event)
    {
        const auto& actualEvent = static_cast<const EntityCollisionEvent&>(event);
        if (actualEvent.GetCollidedEntityIds().first != mPlayerEntityId)
        {
            return;
        }

        const auto& otherEntityPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(actualEvent.GetCollidedEntityIds().second);
        if (otherEntityPhysicsComponent.GetBodyType() == PhysicsComponent::BodyType::DYNAMIC)
        {
            return;
        }

        const auto& playerTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(mPlayerEntityId);
        const auto& playerPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(mPlayerEntityId);
        const auto& otherEntityTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(actualEvent.GetCollidedEntityIds().second);
        if (otherEntityTransformComponent.GetTranslation().y > playerTransformComponent.GetTranslation().y)
        {            
            return;
        }

        // Make sure the player is positioned correctly horizontally on top of the platform
        if (Abs(playerTransformComponent.GetTranslation().x - (otherEntityTransformComponent.GetTranslation().x + otherEntityPhysicsComponent.GetHitBox().mCenterPoint.x)) > otherEntityPhysicsComponent.GetHitBox().mDimensions.x * 0.5f)
        {
            return;
        }
            
        if (Abs(playerPhysicsComponent.GetVelocity().y) > 0.0001f && otherEntityPhysicsComponent.GetBodyType() != PhysicsComponent::BodyType::KINEMATIC)
        {            
            return;
        }

        // If just landed play idle
        if (mJumpsAvailable != mJumpCount)
        {            
            mEntityComponentManager->GetComponent<AnimationComponent>(mPlayerEntityId).PlayAnimation(StringId("idle"));
        }
        
        mJumpsAvailable = mJumpCount;
    });

    mEventCommunicator->RegisterEventCallback<EntityDamagedEvent>([this](const IEvent& event)
    {
        const auto& actualEvent = static_cast<const EntityDamagedEvent&>(event);

        if (mPlayerKilled)
        {
            return;
        }

        if (actualEvent.GetDamagedEntityId() != mPlayerEntityId)
        {
            return;
        }

        if (actualEvent.GetHealthRemaining() > 0.0f)
        {
            const auto& playerPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(mPlayerEntityId);
            const auto& playerTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(mPlayerEntityId);
            mEffectsManager->PlayEffect(playerTransformComponent.GetTranslation() + Vec2ToVec3(playerPhysicsComponent.GetHitBox().mCenterPoint), EffectsManager::EffectType::BLOOD_SPURT_MULTI);
            return;
        }
        
        mPlayerKilled = true;
        mEventCommunicator->DispatchEvent(std::make_unique<PlayerKilledEvent>());
        mEntityComponentManager->GetComponent<AnimationComponent>(mPlayerEntityId).PlayAnimation(StringId("death"), false, false, AnimationComponent::AnimationPriority::HIGH, [this]()
        {
            mEntityComponentManager->RemoveComponent<HealthComponent>(mPlayerEntityId);
        });
    });
}

void PlayerBehaviorController::OnMeleeAttackCooldownTimerTick()
{
    mIsMeleeAttackReady = true;
}

void PlayerBehaviorController::OnFlameBreathCooldownTimerTick()
{
    mIsFlameBreathAttackReady = true;
}

void PlayerBehaviorController::OnBloodDropAnimationTimerTick()
{
    if (mPlayerKilled)
    {
        const auto& playerPhysicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(mPlayerEntityId);
        const auto& playerTransformComponent = mEntityComponentManager->GetComponent<TransformComponent>(mPlayerEntityId);
        mEffectsManager->PlayEffect(playerTransformComponent.GetTranslation() + Vec2ToVec3(playerPhysicsComponent.GetHitBox().mCenterPoint) + glm::vec3(0.0f, -25.0f, 0.0f), EffectsManager::EffectType::BLOOD_SPURT_SINGLE);
    }
}

