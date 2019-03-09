//
//  CastingFlameBreathOnlyEnemyAIComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 09/03/2019.
//

#include "CastingFlameBreathOnlyEnemyAIComponent.h"
#include "../commands/FlameBreathAttackCommand.h"
#include "../commands/SetFacingDirectionCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/DamageComponent.h"
#include "../components/HealthComponent.h"
#include "../components/TransformComponent.h"
#include "../commands/SetVelocityAndAnimateCommand.h"
#include "../rendering/effects/EffectsManager.h"
#include "../events/EventCommunicator.h"
#include "../events/AnnouncePlayerEntityIdEvent.h"
#include "../events/EntityDamagedEvent.h"
#include "../ServiceLocator.h"
#include "../game/Level.h"

const float CastingFlameBreathOnlyEnemyAIComponent::FLAME_BREATH_RECHARGE_DURATION = 2.0f;

CastingFlameBreathOnlyEnemyAIComponent::CastingFlameBreathOnlyEnemyAIComponent(const ServiceLocator& serviceLocator, const EntityId thisEntityId)
    : mServiceLocator(serviceLocator)
    , mEntityComponentManager(mServiceLocator.ResolveService<EntityComponentManager>())
    , mEffectsManager(mServiceLocator.ResolveService<EffectsManager>())
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
    , mThisEntityId(thisEntityId)
    , mTargetEntityId(-1)
    , mFlameBreathRechargeTimer(FLAME_BREATH_RECHARGE_DURATION, [this](){ OnFlameBreathRechargeTimerTick(); })
    , mIsKilled(false)
{
    mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).SetSpecificAnimationDisplacement(StringId("flame_breath"), glm::vec2(100.0f, 20.0f));
    mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).SetSpecificAnimationDisplacement(StringId("melee"), glm::vec2(13.0f, 8.0f));
    mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).SetSpecificAnimationFrameDuration(StringId("flame_breath"), 0.06f);
    
    RegisterEventCallbacks();
}

CastingFlameBreathOnlyEnemyAIComponent::~CastingFlameBreathOnlyEnemyAIComponent()
{
}

void CastingFlameBreathOnlyEnemyAIComponent::VUpdate(const float dt)
{
    if (mIsKilled)
    {
        return;
    }
    
    mFlameBreathRechargeTimer.Update(dt);
    
    const auto& transformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mThisEntityId);
    const auto& targetTransformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mTargetEntityId);
    
    const auto isOtherEntityToTheLeftOfThisOne = targetTransformComponent.GetTranslation().x < transformComponent.GetTranslation().x;
    SetFacingDirectionCommand(mEntityComponentManager, mThisEntityId, isOtherEntityToTheLeftOfThisOne ? FacingDirection::LEFT : FacingDirection::RIGHT).VExecute();
    
}

void CastingFlameBreathOnlyEnemyAIComponent::RegisterEventCallbacks()
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

void CastingFlameBreathOnlyEnemyAIComponent::OnAnnouncePlayerEntityId(const IEvent& event)
{
    mTargetEntityId = static_cast<const AnnouncePlayerEntityIdEvent&>(event).GetPlayerEntityId();
}

void CastingFlameBreathOnlyEnemyAIComponent::OnEntityDamagedEvent(const IEvent& event)
{
    if (mIsKilled)
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
        mEffectsManager.PlayEffect(mEntityComponentManager.GetComponent<TransformComponent>(actualEvent.GetDamageSenderEntityId()).GetTranslation(), EffectsManager::EffectType::BLOOD_SPURT_MULTI);
        return;
    }
    
    mIsKilled = true;
    
    SetVelocityAndAnimateCommand(mEntityComponentManager, mThisEntityId, glm::vec3(0.0f, 0.0f, 0.0f)).VExecute();
    
    const auto& level = mServiceLocator.ResolveService<Level>();
    
    const auto diedFromFlameBreath = level.GetEntityNameFromId(actualEvent.GetDamageSenderEntityId()).GetString() == "flame_breath";
    mEntityComponentManager.GetComponent<AnimationComponent>(mThisEntityId).PlayAnimation(StringId(diedFromFlameBreath ? "flame_death" : "death"), false, false, AnimationComponent::AnimationPriority::HIGH, [this]()
    {
        mEntityComponentManager.RemoveComponent<DamageComponent>(mThisEntityId);
        mEntityComponentManager.RemoveComponent<HealthComponent>(mThisEntityId);
    });
}

void CastingFlameBreathOnlyEnemyAIComponent::OnFlameBreathRechargeTimerTick()
{
    FlameBreathAttackCommand(mServiceLocator, mThisEntityId).VExecute();
}
