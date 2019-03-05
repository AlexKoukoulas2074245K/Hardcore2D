//
//  SetVelocityAndAnimateCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 21/01/2019.
//

#ifndef SetVelocityAndAnimateCommand_h
#define SetVelocityAndAnimateCommand_h

#include "ICommand.h"

#include <glm/vec3.hpp>

class EntityComponentManager;

class SetVelocityAndAnimateCommand final: public ICommand
{
public:
    SetVelocityAndAnimateCommand(EntityComponentManager&, const EntityId, const glm::vec3& velocity);
    void VExecute() override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    
    const EntityId mEntityId;
    const glm::vec3 mVelocity;
};


#endif /* SetVelocityAndAnimateCommand_h */
