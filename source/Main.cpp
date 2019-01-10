//
//  Main.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 05/01/2019.
//

/*
#include "components/DummyComponent.h"
#include "services/ServiceLocator.h"
#include "events/BaseEventCommunicator.h"
#include "components/EntityComponentManager.h"
#include "events/EventCommunicationService.h"
#include "util/SDLCommonUtils.h"
#include "util/Logging.h"
//#include "wingl/Context.h"

#include <iostream>
#include <memory>
#include <cassert>
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>


void testEntityComponentManager(EntityComponentManager& ecms)
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
    EntityComponentManager ecms;
    EventCommunicationService ecs;
    
    sl.RegisterService<EntityComponentManager>(&ecms);
    sl.RegisterService<EventCommunicationService>(&ecs);
    
    testEntityComponentManager(ecms);

    SDL_version imgCompiledVersion;
    SDL_IMAGE_VERSION(&imgCompiledVersion);
    
    const auto* imgLinkedVersion = IMG_Linked_Version();
    
    const auto imgMajorVersionConsistency = imgCompiledVersion.major == imgLinkedVersion->major;
    const auto imgMinorVersionConsistency = imgCompiledVersion.minor == imgLinkedVersion->minor;
    const auto imgPatchConsistency = imgCompiledVersion.patch == imgLinkedVersion->patch;;
    const auto imgVersionConsistency = imgMajorVersionConsistency && imgMinorVersionConsistency && imgPatchConsistency;
    
	const auto sdlImageInitFlags = IMG_INIT_PNG | IMG_INIT_WEBP | IMG_INIT_JPG | IMG_INIT_TIF;	
	if (!imgVersionConsistency || IMG_Init(sdlImageInitFlags) != sdlImageInitFlags)
	{
		ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL_image", "SDL_image did not initialize properly");
		return -1;
	}
    
    Log(LogType::INFO, "Successfully initialized SDL_image version %d.%d.%d", imgCompiledVersion.major, imgCompiledVersion.minor, imgCompiledVersion.patch);
    
    SDL_version mixCompiledVersion;
    SDL_MIXER_VERSION(&mixCompiledVersion);
    
    const auto* mixLinkedVersion = Mix_Linked_Version();
    
    const auto mixMajorVersionConsistency = mixCompiledVersion.major == mixLinkedVersion->major;
    const auto mixMinorVersionConsistency = mixCompiledVersion.minor == mixLinkedVersion->minor;
    const auto mixPatchConsistency = mixCompiledVersion.patch == mixLinkedVersion->patch;
    const auto mixVersionConsistency = mixMajorVersionConsistency && mixMinorVersionConsistency && mixPatchConsistency;
    
	const auto sdlMixerInitFlags = MIX_INIT_OPUS | MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_MOD | MIX_INIT_MID | MIX_INIT_FLAC;
    
	if (!mixVersionConsistency || Mix_Init(sdlMixerInitFlags) != sdlMixerInitFlags)
	{
		ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL_mixer", "SDL_mixer did not initialize properly");
		return -1;
	}
	
	ShowMessageBox(SDL_MESSAGEBOX_INFORMATION, "Dependencies", "SDL_image and SDL_mixer have been initialized properly");

	IMG_Quit();
	Mix_Quit();

    return 0;
}
*/

#include "App.h"

int main(int, char**)
{
    App app;
}

