//
//  FlameBreathAttackCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 09/03/2019.
//

#include "FlameBreathAttackCommand.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/AnimationComponent.h"
#include "../events/EventCommunicator.h"

FlameBreathAttackCommand::FlameBreathAttackCommand(const ServiceLocator& serviceLocator, const EntityId parentEntityId)
    : mServiceLocator(serviceLocator)
    , mEntityComponentManager(mServiceLocator.ResolveService<EntityComponentManager>())
    , mParentEntityId(parentEntityId)
    , mEventCommunicator(mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

void FlameBreathAttackCommand::VExecute()
{
    mEntityComponentManager.GetComponent<AnimationComponent>(mParentEntityId).PlayAnimation(StringId("flamebreath"), false, true, AnimationComponent::AnimationPriority::HIGH);
}
