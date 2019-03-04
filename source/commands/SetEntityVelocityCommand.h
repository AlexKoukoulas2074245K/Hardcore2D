//
//  SetEntityVelocityCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#ifndef SetEntityVelocityCommand_h
#define SetEntityVelocityCommand_h

#include "ICommand.h"

#include <glm/vec3.hpp>

class EntityComponentManager;

class SetEntityVelocityCommand final: public ICommand
{
public:
    SetEntityVelocityCommand(EntityComponentManager&, const EntityId, const float x, const float y, const float z);
    SetEntityVelocityCommand(EntityComponentManager&, const EntityId, const glm::vec3& velocity);
    void VExecute() override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    
    const EntityId mEntityId;
    const glm::vec3 mVelocity;
};


#endif /* SetEntityVelocityCommand_h */
