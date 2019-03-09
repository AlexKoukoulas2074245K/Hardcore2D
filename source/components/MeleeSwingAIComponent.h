//
//  MeleeSwingAIComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 01/02/2019.
//

#ifndef MeleeSwingAIComponent_h
#define MeleeSwingAIComponent_h

#include "IAIComponent.h"
#include "../util/TypeTraits.h"
#include "../util/Timer.h"

#include <memory>

class ServiceLocator;
class EventCommunicator;
class EntityComponentManager;

class MeleeSwingAIComponent final: public IAIComponent
{
public:
    MeleeSwingAIComponent(const ServiceLocator&, const EntityId meleeSwingEntityId, const float timeToLive);
    
    void VUpdate(const float dt) override;
    
private:
    void OnTimeToLiveTimerTick();
    
    const EntityComponentManager& mEntityComponentManager;
    const EntityId mEntityId;
    Timer mTimeToLiveTimer;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
};

#endif /* MeleeSwingAIComponent_h */
