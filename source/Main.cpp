//
//  Main.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 05/01/2019.
//

#include "components/DummyComponent.h"
#include "services/ServiceLocator.h"
#include "services/EntityComponentManagementService.h"
#include "services/EventCommunicationService.h"

#include <memory>
#include <cassert>

void testEntityComponentManagementService(EntityComponentManagementService& ecms)
{
    const auto entityId = ecms.GenerateEntity();
    auto dummyComponent = std::make_unique<DummyComponent>();
    
    ecms.AddComponent<DummyComponent>(entityId, std::move(dummyComponent));
    
    assert(ecms.HasComponent<DummyComponent>(entityId));
    assert(ecms.GetAllComponents<DummyComponent>().size() == 1);
}

int main(int, char**)
{
    ServiceLocator sl;
    EntityComponentManagementService ecms;
    EventCommunicationService ecs;
    
    sl.RegisterService<EntityComponentManagementService>(&ecms);
    sl.RegisterService<EventCommunicationService>(&ecs);
    
    testEntityComponentManagementService(ecms);
    
    return 0;
}

