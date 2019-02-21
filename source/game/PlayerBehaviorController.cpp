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
#include "../events/PlayerRangedAttackEvent.h"
#include "../events/EntityDamagedEvent.h"
#include "../events/AnnouncePlayerEntityIdEvent.h"

const float PlayerBehaviorController::DEFAULT_PLAYER_MELEE_ATTACK_RECHARGE_DURATION = 0.35f;
const float PlayerBehaviorController::DEFAULT_PLAYER_RANGED_ATTACK_RECHARGE_DURATION = 1.0f;
const int PlayerBehaviorController::DEFAULT_RANGED_ATTACK_BATCH_COUNT = 3;

PlayerBehaviorController::PlayerBehaviorController(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
    , mMeleeAttackRechargeDuration(DEFAULT_PLAYER_MELEE_ATTACK_RECHARGE_DURATION)
    , mRangedAttackRechargeDuration(DEFAULT_PLAYER_RANGED_ATTACK_RECHARGE_DURATION)
    , mRangedAttackBatchCount(DEFAULT_RANGED_ATTACK_BATCH_COUNT)
    , mMeleeAttackRechargeTimer(0.0f)
    , mRangedAttackRechargeTimer(0.0f)
    , mRangedAttackCount(DEFAULT_RANGED_ATTACK_BATCH_COUNT)
    , mIsMeleeAttackRecharging(false)
    , mPlayerEntityId(-1)
{
    
}

bool PlayerBehaviorController::VInitialize()
{
    mEventCommunicator = mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    RegisterEventCallbacks();
    return true;
}

void PlayerBehaviorController::Update(const float dt)
{
    if (mIsMeleeAttackRecharging)
    {
        mMeleeAttackRechargeTimer += dt;
        if (mMeleeAttackRechargeTimer > mMeleeAttackRechargeDuration)
        {
            mMeleeAttackRechargeTimer = 0.0f;
            mIsMeleeAttackRecharging = false;
        }
    }
    
    
    mRangedAttackRechargeTimer += dt;
    if (mRangedAttackRechargeTimer > mRangedAttackRechargeDuration)
    {
        mRangedAttackRechargeTimer = 0.0f;
        mRangedAttackCount++;
        if (mRangedAttackCount > mRangedAttackBatchCount)
        {
            mRangedAttackCount = mRangedAttackBatchCount;
        }
    }
}

bool PlayerBehaviorController::IsMeleeAttackAvailable() const
{
    return !mIsMeleeAttackRecharging;
}

bool PlayerBehaviorController::IsRangedAttackAvailable() const
{
    return mRangedAttackCount > 0;
}

void PlayerBehaviorController::RegisterEventCallbacks()
{
    mEventCommunicator->RegisterEventCallback<PlayerMeleeAttackEvent>([this](const IEvent&)
    {
        mIsMeleeAttackRecharging = true;
    });
    mEventCommunicator->RegisterEventCallback<PlayerRangedAttackEvent>([this](const IEvent&)
    {
        mRangedAttackRechargeTimer = 0.0f;
        if (--mRangedAttackCount < 0)
        {
            mRangedAttackCount = 0;
        }
    });
    mEventCommunicator->RegisterEventCallback<AnnouncePlayerEntityIdEvent>([this](const IEvent& event)
    {
        mPlayerEntityId = static_cast<const AnnouncePlayerEntityIdEvent&>(event).GetPlayerEntityId();
    });
    mEventCommunicator->RegisterEventCallback<EntityDamagedEvent>([this](const IEvent& event)
    {
        const auto& actualEvent = static_cast<const EntityDamagedEvent&>(event);
        if (actualEvent.GetDamagedEntityId() == mPlayerEntityId)
        {

        }
    });
}


