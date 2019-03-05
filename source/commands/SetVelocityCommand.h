//
//  SetVelocityCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#ifndef SetVelocityCommand_h
#define SetVelocityCommand_h

#include "ICommand.h"

#include <glm/vec3.hpp>

class EntityComponentManager;

class SetVelocityCommand final: public ICommand
{
public:
    SetVelocityCommand(EntityComponentManager&, const EntityId, const float x, const float y, const float z);
    SetVelocityCommand(EntityComponentManager&, const EntityId, const glm::vec3& velocity);
    void VExecute() override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    
    const EntityId mEntityId;
    const glm::vec3 mVelocity;
};


#endif /* SetVelocityCommand_h */
