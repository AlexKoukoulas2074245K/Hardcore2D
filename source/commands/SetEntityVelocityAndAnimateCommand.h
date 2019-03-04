//
//  SetEntityVelocityAndAnimateCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#ifndef SetEntityVelocityAndAnimateCommand_h
#define SetEntityVelocityAndAnimateCommand_h

#include "ICommand.h"

#include <glm/vec3.hpp>

class EntityComponentManager;

class SetEntityVelocityAndAnimateCommand final: public ICommand
{
public:
    SetEntityVelocityAndAnimateCommand(EntityComponentManager&, const EntityId, const glm::vec3& velocity);
    void VExecute() override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    
    const EntityId mEntityId;
    const glm::vec3 mVelocity;
};


#endif /* SetEntityVelocityAndAnimateCommand_h */
