//
//  BloodDropAIComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/02/2019.
//

#ifndef BloodDropAIComponent_h
#define BloodDropAIComponent_h

#include "IAIComponent.h"
#include "../util/TypeTraits.h"

#include <memory>

class ServiceLocator;
class EventCommunicator;
class EntityComponentManager;

class BloodDropAIComponent final: public IAIComponent
{
public:
    BloodDropAIComponent(const ServiceLocator&, const EntityId bloodDropEntityId, const float timeToLive);
    
    void VUpdate(const float dt) override;
    
private:
    const EntityComponentManager& mEntityComponentManager;
    const EntityId mEntityId;
    float mTimeToLive;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
};

#endif /* BloodDropAIComponent_h */
