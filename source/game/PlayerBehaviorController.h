//
//  PlayerBehaviorController.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 06/02/2019.
//

#ifndef PlayerBehaviorController_h
#define PlayerBehaviorController_h

#include "../IService.h"
#include "../util/TypeTraits.h"

#include <memory>

class ServiceLocator;
class EventCommunicator;

class PlayerBehaviorController final: public IService
{
    friend class App;
public:
    bool VInitialize() override;
    void Update(const float dt);
    
    bool IsMeleeAttackAvailable() const;
    bool IsRangedAttackAvailable() const;
    
private:
    static const float DEFAULT_PLAYER_MELEE_ATTACK_RECHARGE_DURATION;
    static const float DEFAULT_PLAYER_RANGED_ATTACK_RECHARGE_DURATION;
    static const int DEFAULT_RANGED_ATTACK_BATCH_COUNT;
    
    PlayerBehaviorController(const ServiceLocator&);
    
    void RegisterEventCallbacks();

    const ServiceLocator& mServiceLocator;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
    EntityId mPlayerEntityId;

    float mMeleeAttackRechargeDuration;
    float mRangedAttackRechargeDuration;
    int mRangedAttackBatchCount;
    
    float mMeleeAttackRechargeTimer;
    float mRangedAttackRechargeTimer;
    int mRangedAttackCount;
    
    bool mIsMeleeAttackRecharging;
};

#endif /* PlayerBehaviorController_h */
