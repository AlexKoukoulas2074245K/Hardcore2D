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
#include "services/ServiceLocator.h"
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

void App::Update(const float dt)
{
    
}

void App::CreateAndRegisterServices()
{
    mServiceLocator = std::make_unique<ServiceLocator>();
    mEntityComponentManager = std::make_unique<EntityComponentManager>();
    mEventCommunicationService = std::make_unique<EventCommunicationService>();
    mCoreEngineService = std::make_unique<CoreEngineService>();
    
    if (!mCoreEngineService->InitializeContext()) return;
    
    mServiceLocator->RegisterService<EntityComponentManager>(mEntityComponentManager.get());
    mServiceLocator->RegisterService<EventCommunicationService>(mEventCommunicationService.get());
    mServiceLocator->RegisterService<CoreEngineService>(mCoreEngineService.get());
}
