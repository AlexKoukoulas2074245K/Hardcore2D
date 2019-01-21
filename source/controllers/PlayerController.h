//
//  PlayerController.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#ifndef PlayerController_h
#define PlayerController_h

#include "../input/InputHandler.h"
#include "../util/TypeTraits.h"

class EntityComponentManager;

class PlayerController final
{
public:
    PlayerController(EntityComponentManager&, const EntityId);
    
    bool ConsumeInputAction(const InputHandler::InputAction&);
    
private:
    EntityComponentManager& mEntityComponentManager;
    EntityId mEntityId;
};

#endif /* PlayerController_h */
