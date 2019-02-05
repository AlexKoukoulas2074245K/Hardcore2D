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
    const EntityComponentManager& mEntityComponentManager;
    const EntityId mEntityId;
    float mTimeToLive;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
};

#endif /* RangedShurikenAIComponent_h */
