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
#include "../util/Timer.h"

#include <memory>

class ServiceLocator;
class EventCommunicator;
class EntityComponentManager;
class EffectsManager;

class PlayerBehaviorController final: public IService
{
    friend class App;
public:
    bool VInitialize() override;
    void Update(const float dt);
    
    bool IsOnAir() const;
    bool IsImmobilized() const;
    bool CanJump() const;
    bool IsMeleeAttackAvailable() const;
    bool IsFlameBreathAttackAvailable() const;
    
private:
    static const float DEFAULT_PLAYER_MELEE_ATTACK_RECHARGE_DURATION;
    static const float DEFAULT_PLAYER_FLAME_BREATH_ATTACK_COOLDOWN_DURATION;
    static const float BLOOD_DROP_DEATH_ANIMATION_COOLDOWN;
    static const int DEFAULT_JUMP_COUNT;

    PlayerBehaviorController(const ServiceLocator&);
    
    void RegisterEventCallbacks();
    void OnMeleeAttackCooldownTimerTick();
    void OnFlameBreathCooldownTimerTick();
    void OnBloodDropAnimationTimerTick();
    
    
    const ServiceLocator& mServiceLocator;
    EntityComponentManager* mEntityComponentManager;
    EffectsManager* mEffectsManager;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
    EntityId mPlayerEntityId;

    int mJumpCount;
    int mJumpsAvailable;

    bool mIsMeleeAttackReady;
    bool mIsFlameBreathAttackReady;
    bool mPlayerKilled;
    bool mIsCastingFlameBreath;
    
    Timer mMeleeAttackCooldownTimer;
    Timer mFlameBreathCooldownTimer;
    Timer mBloodDropAnimationTimer;
};

#endif /* PlayerBehaviorController_h */
