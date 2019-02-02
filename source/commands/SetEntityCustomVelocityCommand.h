//
//  SetEntityCustomVelocityCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#ifndef SetEntityCustomVelocityCommand_h
#define SetEntityCustomVelocityCommand_h

#include "ICommand.h"

#include <glm/vec3.hpp>

class EntityComponentManager;

class SetEntityCustomVelocityCommand final: public ICommand
{
public:
    SetEntityCustomVelocityCommand(EntityComponentManager&, const EntityId, const glm::vec3& velocity);
    void Execute() override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    
    const EntityId mEntityId;
    const glm::vec3 mVelocity;
};


#endif /* SetEntityCustomVelocityCommand_h */
