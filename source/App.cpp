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
    mServiceLocator = std::unique_ptr<ServiceLocator>(new ServiceLocator);
    mEntityComponentManager = std::unique_ptr<EntityComponentManager>(new EntityComponentManager);
    mEventCommunicationService = std::unique_ptr<EventCommunicationService>(new EventCommunicationService);
    mCoreEngineService = std::unique_ptr<CoreEngineService>(new CoreEngineService);
    mResourceManager = std::unique_ptr<ResourceManager>(new ResourceManager("../../res"));
    
    if (!mCoreEngineService->InitializeEngine()) return;
    if (!mResourceManager->InitializeResourceLoaders()) return;
    
    mServiceLocator->RegisterService<EntityComponentManager>(mEntityComponentManager.get());
    mServiceLocator->RegisterService<EventCommunicationService>(mEventCommunicationService.get());
    mServiceLocator->RegisterService<CoreEngineService>(mCoreEngineService.get());
    mServiceLocator->RegisterService<ResourceManager>(mResourceManager.get());
}
