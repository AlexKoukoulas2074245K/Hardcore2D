//
//  SetEntityFacingDirectionCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#ifndef SetEntityFacingDirectionCommand_h
#define SetEntityFacingDirectionCommand_h

#include "ICommand.h"
#include "../game/GameConstants.h"

class EntityComponentManager;

class SetEntityFacingDirectionCommand final: public ICommand
{
public:
    static const StringId COMMAND_CLASS_ID;
    
    SetEntityFacingDirectionCommand(EntityComponentManager&, const EntityId, const FacingDirection);
    void Execute() override;
    StringId GetCommandClassId() const override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    
    const EntityId mEntityId;
    const FacingDirection mFacingDirection;
};

#endif /* SetEntityFacingDirectionCommand_h */
