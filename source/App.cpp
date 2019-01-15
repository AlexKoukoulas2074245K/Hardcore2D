//
//  App.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "App.h"
#include "components/EntityComponentManager.h"
#include "events/EventCommunicationService.h"
#include "engine/CoreRenderingService.h"
#include "ServiceLocator.h"
#include "resources/ResourceManager.h"
#include "util/Logging.h"
#include "gl/Context.h"
#include "resources/TextureResource.h"

#ifdef _WIN32
static const char* RES_ROOT = "../res/";
#else
static const char* RES_ROOT = "../../res/";
#endif

App::App()
{
}

App::~App()
{
    
}

void App::Run()
{
    mCoreRenderingService->GameLoop([this](const float dt){ Update(dt); });
}

void App::Update(const float)
{
    //auto& textureResource = static_cast<TextureResource&>(mResourceManager->GetResource("jungle-sky.png"));
    
    //const auto width = mCoreRenderingService->GetRenderableWidth();
    //const auto height = mCoreRenderingService->GetRenderableHeight();
    
}

bool App::Initialize()
{
    mServiceLocator = std::unique_ptr<ServiceLocator>(new ServiceLocator);
    mEntityComponentManager = std::unique_ptr<EntityComponentManager>(new EntityComponentManager);
    mEventCommunicationService = std::unique_ptr<EventCommunicationService>(new EventCommunicationService);
    mCoreRenderingService = std::unique_ptr<CoreRenderingService>(new CoreRenderingService(*mServiceLocator));	
    mResourceManager = std::unique_ptr<ResourceManager>(new ResourceManager("../res/"));
    
    mServiceLocator->RegisterService<EntityComponentManager>(mEntityComponentManager.get());
    mServiceLocator->RegisterService<EventCommunicationService>(mEventCommunicationService.get());
    mServiceLocator->RegisterService<CoreRenderingService>(mCoreRenderingService.get());
    mServiceLocator->RegisterService<ResourceManager>(mResourceManager.get());
    
    if (!mCoreRenderingService->InitializeEngine()) return false;
    if (!mResourceManager->InitializeResourceLoaders()) return false;
    
    
    mResourceManager->LoadResource("jungle-sky.png");
    return true;
}
