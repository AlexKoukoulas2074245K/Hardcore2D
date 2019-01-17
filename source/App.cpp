//
//  App.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "App.h"
#include "components/EntityComponentManager.h"
#include "components/TransformationComponent.h"
#include "components/AnimationComponent.h"
#include "components/ShaderComponent.h"
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

void App::Update(const float dt)
{
    //mEntityComponentManager->GetComponent<TransformationComponent>(mNinjaId).mTranslation.x += 0.2 * dt;
    //mEntityComponentManager->GetComponent<TransformationComponent>(mNinjaId).mRotation.z += 0.2 * dt;
    mEntityComponentManager->GetComponent<TransformationComponent>(mNinjaId).mScale.x += 0.02 * dt;
    mEntityComponentManager->GetComponent<TransformationComponent>(mNinjaId).mScale.y += 0.02 * dt;
    
    mCoreRenderingService->RenderEntity(mBackgroundId);
    mCoreRenderingService->RenderEntity(mNinjaId);
}

bool App::Initialize()
{
    mServiceLocator = std::unique_ptr<ServiceLocator>(new ServiceLocator);
    mEntityComponentManager = std::unique_ptr<EntityComponentManager>(new EntityComponentManager);
    mEventCommunicationService = std::unique_ptr<EventCommunicationService>(new EventCommunicationService);
    mResourceManager = std::unique_ptr<ResourceManager>(new ResourceManager(RES_ROOT));
    mCoreRenderingService = std::unique_ptr<CoreRenderingService>(new CoreRenderingService(*mServiceLocator));
    
    mServiceLocator->RegisterService<EntityComponentManager>(mEntityComponentManager.get());
    mServiceLocator->RegisterService<EventCommunicationService>(mEventCommunicationService.get());
    mServiceLocator->RegisterService<CoreRenderingService>(mCoreRenderingService.get());
    mServiceLocator->RegisterService<ResourceManager>(mResourceManager.get());
    
    if (!mCoreRenderingService->InitializeEngine()) return false;
    if (!mResourceManager->InitializeResourceLoaders()) return false;
    
    auto ninjaTextureResourceId = mResourceManager->LoadResource("ninja.png");
    auto backgroundTextureResourceId = mResourceManager->LoadResource("jungle-sky.png");
    
    auto ninjaTextureId = mResourceManager->GetResource<TextureResource>(ninjaTextureResourceId).GetGLTextureId();
    auto backgroundTextureId = mResourceManager->GetResource<TextureResource>(backgroundTextureResourceId).GetGLTextureId();
    
    
    mNinjaId = mEntityComponentManager->GenerateEntity();
    
    auto ninjaTransformComponent = std::make_unique<TransformationComponent>();
    ninjaTransformComponent->mScale = glm::vec3(0.5f, 0.5f, 1.0f);
    auto ninjaAnimationComponent = std::make_unique<AnimationComponent>(std::vector{ninjaTextureId});
    auto ninjaShaderComponent = std::make_unique<ShaderComponent>("basic");

    
    mEntityComponentManager->AddComponent<TransformationComponent>(mNinjaId, std::move(ninjaTransformComponent));
    mEntityComponentManager->AddComponent<AnimationComponent>(mNinjaId, std::move(ninjaAnimationComponent));
    mEntityComponentManager->AddComponent<ShaderComponent>(mNinjaId, std::move(ninjaShaderComponent));
    
    mBackgroundId = mEntityComponentManager->GenerateEntity();
    
    auto backgroundTransformComponent = std::make_unique<TransformationComponent>();
    auto backgroundAnimationComponent = std::make_unique<AnimationComponent>(std::vector{backgroundTextureId});
    auto backgroundShaderComponent = std::make_unique<ShaderComponent>("basic");
    
    mEntityComponentManager->AddComponent<TransformationComponent>(mBackgroundId, std::move(backgroundTransformComponent));
    mEntityComponentManager->AddComponent<AnimationComponent>(mBackgroundId, std::move(backgroundAnimationComponent));
    mEntityComponentManager->AddComponent<ShaderComponent>(mBackgroundId, std::move(backgroundShaderComponent));
    
    return true;
}
