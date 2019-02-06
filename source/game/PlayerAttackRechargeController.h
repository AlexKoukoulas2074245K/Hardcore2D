//
//  PlayerAttackRechargeController.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 06/02/2019.
//

#ifndef PlayerAttackRechargeController_h
#define PlayerAttackRechargeController_h

#include "../IService.h"

#include <memory>

class ServiceLocator;
class EventCommunicator;

class PlayerAttackRechargeController final: public IService
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
    
    PlayerAttackRechargeController(const ServiceLocator&);
    
    const ServiceLocator& mServiceLocator;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
    float mMeleeAttackRechargeDuration;
    float mRangedAttackRechargeDuration;
    int mRangedAttackBatchCount;
    
    float mMeleeAttackRechargeTimer;
    float mRangedAttackRechargeTimer;
    int mRangedAttackCount;
    
    bool mIsMeleeAttackRecharging;
};

#endif /* PlayerAttackRechargeController_h */
