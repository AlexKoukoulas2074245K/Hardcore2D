//
//  PlayerInputActionConsumer.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#ifndef PlayerInputActionConsumer_h
#define PlayerInputActionConsumer_h

#include "IInputActionConsumer.h"
#include "../util/TypeTraits.h"

#include <memory>

class ServiceLocator;
class EntityComponentManager;
class EventCommunicator;

class PlayerInputActionConsumer final: public IInputActionConsumer
{
public:
    PlayerInputActionConsumer(const ServiceLocator&, const EntityId);
    ~PlayerInputActionConsumer();
    
    bool VConsumeInputAction(const InputAction&) const override;
    
private:
    const ServiceLocator& mServiceLocator;
    EntityComponentManager& mEntityComponentManager;
    EntityId mEntityId;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
};

#endif /* PlayerInputActionConsumer_h */
