//
//  RangedShurikenAIComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 05/02/2019.
//

#ifndef RangedShurikenAIComponent_h
#define RangedShurikenAIComponent_h

#include "IAIComponent.h"
#include "../util/TypeTraits.h"
#include "../util/Timer.h"

#include <memory>

class ServiceLocator;
class EventCommunicator;
class EntityComponentManager;

class RangedShurikenAIComponent final: public IAIComponent
{
public:
    RangedShurikenAIComponent(const ServiceLocator&, const EntityId rangedShurikenEntityId, const float timeToLive);
    
    void VUpdate(const float dt) override;
    
private:
    void OnTimeToLiveTimerTick();
    
    EntityComponentManager& mEntityComponentManager;
    const EntityId mEntityId;
    Timer mTimeToLiveTimer;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
};

#endif /* RangedShurikenAIComponent_h */
