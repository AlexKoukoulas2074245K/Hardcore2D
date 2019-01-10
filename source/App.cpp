//
//  App.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "App.h"
#include "components/EntityComponentManager.h"
#include "events/EventCommunicationService.h"
#include "engine/CoreEngineService.h"
#include "ServiceLocator.h"
#include "resources/ResourceManager.h"
#include "util/Logging.h"

App::App()
{
    CreateAndRegisterServices();
    Run();
}

App::~App()
{
    
}

void App::Run()
{
    mCoreEngineService->GameLoop([this](const float dt){ Update(dt); });
}

void App::Update(const float)
{
    
}

void App::CreateAndRegisterServices()
{
    mServiceLocator = std::make_unique<ServiceLocator>();
    mEntityComponentManager = std::make_unique<EntityComponentManager>();
    mEventCommunicationService = std::make_unique<EventCommunicationService>();
    mCoreEngineService = std::make_unique<CoreEngineService>();
    mResourceManager = std::make_unique<ResourceManager>();
    
    if (!mCoreEngineService->InitializeContext()) return;
    
    mServiceLocator->RegisterService<EntityComponentManager>(mEntityComponentManager.get());
    mServiceLocator->RegisterService<EventCommunicationService>(mEventCommunicationService.get());
    mServiceLocator->RegisterService<CoreEngineService>(mCoreEngineService.get());
    mServiceLocator->RegisterService<ResourceManager>(mResourceManager.get());
    
    mResourceManager->LoadResource("aasdkadl.png");
    mResourceManager->LoadResource("asdad.json");
    mResourceManager->GetResource("asdad.json");
    mResourceManager->GetResource("asd.json");
}
