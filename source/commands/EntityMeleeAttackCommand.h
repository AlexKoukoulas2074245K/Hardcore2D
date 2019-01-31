//
//  EntityMeleeAttackCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#ifndef EntityMeleeAttackCommand_h
#define EntityMeleeAttackCommand_h

#include "ICommand.h"
#include "../util/TypeTraits.h"

class EntityComponentManager;

class EntityMeleeAttackCommand final: public ICommand
{
public:
    static const StringId COMMAND_CLASS_ID;
    
    EntityMeleeAttackCommand(EntityComponentManager&, const EntityId);
    void Execute() override;
    StringId GetCommandClassId() const override;
    
private:
    EntityComponentManager& mEntityComponentManager;
    
    const EntityId mEntityId;    
};


#endif /* EntityMeleeAttackCommand_h */
