//
//  PlayerInputController.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#ifndef PlayerInputController_h
#define PlayerInputController_h

#include "../InputHandler.h"
#include "../util/TypeTraits.h"

class EntityComponentManager;

class PlayerInputController final
{
public:
    PlayerInputController(EntityComponentManager&, const EntityId);
    
    bool ConsumeInputAction(const InputHandler::InputAction&);
    
private:
    EntityComponentManager& mEntityComponentManager;
    EntityId mEntityId;
};

#endif /* PlayerInputController_h */
