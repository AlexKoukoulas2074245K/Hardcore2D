//
//  EntityRangedShurikenAttackCommand.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#ifndef EntityRangedShurikenAttackCommand_h
#define EntityRangedShurikenAttackCommand_h

#include "ICommand.h"
#include "../util/TypeTraits.h"

class ServiceLocator;
class EntityComponentManager;
class ResourceManager;

class EntityRangedShurikenAttackCommand final: public ICommand
{
public:
    EntityRangedShurikenAttackCommand(const ServiceLocator&, const EntityId);
    
    void VExecute() override;
private:
    EntityComponentManager& mEntityComponentManager;
    ResourceManager& mResourceManager;
    
    const EntityId mEntityId;
};

#endif /* EntityRangedShurikenAttackCommand_h */
