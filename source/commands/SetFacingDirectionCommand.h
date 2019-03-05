//
//  SetFacingDirectionCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#ifndef SetFacingDirectionCommand_h
#define SetFacingDirectionCommand_h

#include "ICommand.h"
#include "../game/GameTypeTraits.h"
#include "../game/GameConstants.h"

class EntityComponentManager;

class SetFacingDirectionCommand final: public ICommand
{
public:
    SetFacingDirectionCommand(EntityComponentManager&, const EntityId, const FacingDirection);
    void VExecute() override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    
    const EntityId mEntityId;
    const FacingDirection mFacingDirection;
};

#endif /* SetFacingDirectionCommand_h */
