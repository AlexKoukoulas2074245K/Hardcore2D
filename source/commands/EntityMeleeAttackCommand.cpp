//
//  EntityMeleeAttackCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#include "EntityMeleeAttackCommand.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"
#include "../util/MathUtils.h"

const StringId EntityMeleeAttackCommand::COMMAND_CLASS_ID("EntityMeleeAttackCommand");

EntityMeleeAttackCommand::EntityMeleeAttackCommand(EntityComponentManager& entityComponentManager, const EntityId entityId)
: mEntityComponentManager(entityComponentManager)
, mEntityId(entityId)
{
    
}

void EntityMeleeAttackCommand::Execute()
{
    auto& entityAnimationComponent = mEntityComponentManager.GetComponent<AnimationComponent>(mEntityId);
    entityAnimationComponent.PlayAnimationOnce(StringId("melee"));


}

StringId EntityMeleeAttackCommand::GetCommandClassId() const
{
    return COMMAND_CLASS_ID;
}
