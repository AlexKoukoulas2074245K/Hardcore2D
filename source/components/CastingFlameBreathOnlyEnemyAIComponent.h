//
//  CastingFlameBreathOnlyEnemyAIComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 09/03/2019.
//

#ifndef CastingFlameBreathOnlyEnemyAIComponent_h
#define CastingFlameBreathOnlyEnemyAIComponent_h

#include "IAIComponent.h"
#include "../util/TypeTraits.h"
#include "../util/Timer.h"

#include <memory>

class ServiceLocator;
class EntityComponentManager;
class EffectsManager;
class EventCommunicator;
class IEvent;

class CastingFlameBreathOnlyEnemyAIComponent final: public IAIComponent
{
public:
    CastingFlameBreathOnlyEnemyAIComponent(const ServiceLocator&, const EntityId thisEntityId);
    ~CastingFlameBreathOnlyEnemyAIComponent();
    
    void VUpdate(const float dt) override;

private:
    static const float FLAME_BREATH_RECHARGE_DURATION;
    
    void RegisterEventCallbacks();
    void OnAnnouncePlayerEntityId(const IEvent& event);
    void OnEntityDamagedEvent(const IEvent& event);
    void OnFlameBreathRechargeTimerTick();
    
    const ServiceLocator& mServiceLocator;
    EntityComponentManager& mEntityComponentManager;
    EffectsManager& mEffectsManager;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    const EntityId mThisEntityId;
    EntityId mTargetEntityId;
    Timer mFlameBreathRechargeTimer;
    bool mIsKilled;
};

#endif /* CastingFlameBreathOnlyEnemyAIComponent_h */
