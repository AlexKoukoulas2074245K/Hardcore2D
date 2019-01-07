//
//  Main.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 05/01/2019.
//

#include "Main.h"
#include "components/DummyComponent.h"
#include "services/ServiceLocator.h"
#include "services/EntityComponentManagementService.h"

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
    sl.RegisterService<EntityComponentManagementService>(&ecms);
    
    testEntityComponentManagementService(ecms);
    
    return 0;
}

