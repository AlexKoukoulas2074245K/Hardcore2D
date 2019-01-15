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
#include "gl/Context.h"
#include "resources/TextureResource.h"

App::App()
{
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
    auto& textureResource = static_cast<TextureResource&>(mResourceManager->GetResource("jungle-sky.png"));
    
    GL_CHECK(Enable(GL_TEXTURE_2D));
    GL_CHECK(BindTexture(GL_TEXTURE_2D, textureResource.GetGlTextureId()));
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);glVertex2f(-1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex2f(-1.0f, -1.0f);
    glEnd();
    GL_CHECK(Disable(GL_TEXTURE_2D));
}

bool App::Initialize()
{
    mServiceLocator = std::unique_ptr<ServiceLocator>(new ServiceLocator);
    mEntityComponentManager = std::unique_ptr<EntityComponentManager>(new EntityComponentManager);
    mEventCommunicationService = std::unique_ptr<EventCommunicationService>(new EventCommunicationService);
    mCoreEngineService = std::unique_ptr<CoreEngineService>(new CoreEngineService);
    mResourceManager = std::unique_ptr<ResourceManager>(new ResourceManager("../../res/"));
    
    if (!mCoreEngineService->InitializeEngine()) return false;
    if (!mResourceManager->InitializeResourceLoaders()) return false;
    
    mServiceLocator->RegisterService<EntityComponentManager>(mEntityComponentManager.get());
    mServiceLocator->RegisterService<EventCommunicationService>(mEventCommunicationService.get());
    mServiceLocator->RegisterService<CoreEngineService>(mCoreEngineService.get());
    mServiceLocator->RegisterService<ResourceManager>(mResourceManager.get());
    
    mResourceManager->LoadResource("jungle-sky.png");
    return true;
}
