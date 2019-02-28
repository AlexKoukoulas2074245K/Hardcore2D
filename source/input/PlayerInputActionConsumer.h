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
class PlayerBehaviorController;
class EventCommunicator;

class PlayerInputActionConsumer final: public IInputActionConsumer
{
public:
    PlayerInputActionConsumer(const ServiceLocator&, const EntityId);
    ~PlayerInputActionConsumer();
    
    bool VConsumeInputAction(const InputAction&) const override;
    
private:
    void RegisterEventCallbacks();

    const ServiceLocator& mServiceLocator;
    const PlayerBehaviorController& mPlayerBehaviorController;
    
    EntityComponentManager& mEntityComponentManager;
    std::unique_ptr<EventCommunicator> mEventCommunicator;

    EntityId mEntityId;
    bool mPlayerKilled;
};

#endif /* PlayerInputActionConsumer_h */
