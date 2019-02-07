//
//  PlayerAttackRechargeController.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 06/02/2019.
//

#include "PlayerAttackRechargeController.h"
#include "../ServiceLocator.h"
#include "../events/EventCommunicator.h"
#include "../events/PlayerMeleeAttackEvent.h"
#include "../events/PlayerRangedAttackEvent.h"

const float PlayerAttackRechargeController::DEFAULT_PLAYER_MELEE_ATTACK_RECHARGE_DURATION = 0.35f;
const float PlayerAttackRechargeController::DEFAULT_PLAYER_RANGED_ATTACK_RECHARGE_DURATION = 1.0f;
const int PlayerAttackRechargeController::DEFAULT_RANGED_ATTACK_BATCH_COUNT = 3;

PlayerAttackRechargeController::PlayerAttackRechargeController(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
    , mMeleeAttackRechargeDuration(DEFAULT_PLAYER_MELEE_ATTACK_RECHARGE_DURATION)
    , mRangedAttackRechargeDuration(DEFAULT_PLAYER_RANGED_ATTACK_RECHARGE_DURATION)
    , mRangedAttackBatchCount(DEFAULT_RANGED_ATTACK_BATCH_COUNT)
    , mMeleeAttackRechargeTimer(0.0f)
    , mRangedAttackRechargeTimer(0.0f)
    , mRangedAttackCount(DEFAULT_RANGED_ATTACK_BATCH_COUNT)
    , mIsMeleeAttackRecharging(false)
{
    
}

bool PlayerAttackRechargeController::VInitialize()
{
    mEventCommunicator = mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
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

    return true;
}

void PlayerAttackRechargeController::Update(const float dt)
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

bool PlayerAttackRechargeController::IsMeleeAttackAvailable() const
{
    return !mIsMeleeAttackRecharging;
}

bool PlayerAttackRechargeController::IsRangedAttackAvailable() const
{
    return mRangedAttackCount > 0;
}


