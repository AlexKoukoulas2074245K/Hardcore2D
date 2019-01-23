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

class EntityComponentManager;

class PlayerInputActionConsumer final: public IInputActionConsumer
{
public:
    PlayerInputActionConsumer(EntityComponentManager&, const EntityId);
    
    bool VConsumeInputAction(const InputAction&) const override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    EntityId mEntityId;
};

#endif /* PlayerInputActionConsumer_h */
