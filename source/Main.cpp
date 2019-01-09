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
#include <SDL_mixer.h>


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

	const auto sdlImageInitFlags = IMG_INIT_PNG | IMG_INIT_WEBP | IMG_INIT_JPG | IMG_INIT_TIF;	
	if (IMG_Init(sdlImageInitFlags) != sdlImageInitFlags)
	{
		ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL_image", "SDL_image did not initialize properly");
		return -1;
	}
	
	const auto sdlMixerInitFlags = MIX_INIT_OPUS | MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_MOD | MIX_INIT_MID | MIX_INIT_FLAC;
	if (Mix_Init(sdlMixerInitFlags) != sdlMixerInitFlags)
	{
		ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL_mixer", "SDL_mixer did not initialize properly");
		return -1;
	}
	
	ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "Dependencies", "SDL_image and SDL_mixer have been initialized correctly");

	IMG_Quit();
	Mix_Quit();

    return 0;
}

