//
//  FlameBreathAIComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 09/03/2019.
//

#ifndef FlameBreathAIComponent_h
#define FlameBreathAIComponent_h

#include "IAIComponent.h"
#include "../util/TypeTraits.h"
#include "../util/Timer.h"

#include <memory>

class ServiceLocator;
class EventCommunicator;
class EntityComponentManager;

class FlameBreathAIComponent final: public IAIComponent
{
public:
    FlameBreathAIComponent(const ServiceLocator&, const EntityId parentEntityId, const EntityId flameBreathEntityId, const float timeToLive);
    
    void VUpdate(const float dt) override;
    
private:
    void OnTimeToLiveTimerTick();
    
    const EntityComponentManager& mEntityComponentManager;
    const EntityId mParentEntityId;
    const EntityId mEntityId;
    Timer mTimeToLiveTimer;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
};

#endif /* FlameBreathAIComponent_h */
