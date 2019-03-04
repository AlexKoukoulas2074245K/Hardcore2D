//
//  SetAngularVelocityCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 04/03/2019.
//

#ifndef SetAngularVelocityCommand_h
#define SetAngularVelocityCommand_h

#include "ICommand.h"

class EntityComponentManager;

class SetAngularVelocityCommand final: public ICommand
{
public:
    SetAngularVelocityCommand(EntityComponentManager&, const EntityId, const float angularVelocity);
    void VExecute() override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    
    const EntityId mEntityId;
    const float mAngularVelocity;
};

#endif /* SetAngularVelocityCommand_h */
