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
#include "SDLCommon.h"
//#include "wingl/Context.h"

#include <memory>
#include <cassert>
#include <iostream>
#include <SDL_image.h>

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
	IMG_Init(IMG_INIT_PNG);
	IMG_Quit();
	ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL_image", "SDL_image has been initialized correctly");
    return 0;
}

