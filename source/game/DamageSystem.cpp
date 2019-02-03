//
//  DamageSystem.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 02/02/2019.
//

#include "DamageSystem.h"
#include "../ServiceLocator.h"
#include "../events/EventCommunicator.h"
#include "../components/EntityComponentManager.h"

DamageSystem::DamageSystem(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
    
}

bool DamageSystem::Initialize()
{
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
    mEventCommunicator = mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    
    return true;
}
