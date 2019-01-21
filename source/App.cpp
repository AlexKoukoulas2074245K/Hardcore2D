//
//  App.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "App.h"
#include "controllers/PlayerController.h"
#include "components/EntityComponentManager.h"
#include "components/TransformationComponent.h"
#include "components/AnimationComponent.h"
#include "components/ShaderComponent.h"
#include "components/PhysicsComponent.h"
#include "events/EventCommunicationService.h"
#include "rendering/CoreRenderingService.h"
#include "physics/PhysicsSystem.h"
#include "ServiceLocator.h"
#include "resources/ResourceManager.h"
#include "util/Logging.h"
#include "gl/Context.h"
#include "resources/TextureResource.h"
#include "input/InputHandler.h"

#include <vector>

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
	mCoreRenderingService->GameLoop([this](const float dt) { Update(dt); });
}

bool App::Initialize()
{
    mServiceLocator = std::unique_ptr<ServiceLocator>(new ServiceLocator);
    mEntityComponentManager = std::unique_ptr<EntityComponentManager>(new EntityComponentManager);
    mEventCommunicationService = std::unique_ptr<EventCommunicationService>(new EventCommunicationService);
    mResourceManager = std::unique_ptr<ResourceManager>(new ResourceManager(RES_ROOT));
    mCoreRenderingService = std::unique_ptr<CoreRenderingService>(new CoreRenderingService(*mServiceLocator));
    mPhysicsSystem = std::unique_ptr<PhysicsSystem>(new PhysicsSystem(*mServiceLocator));
    mInputHandler = std::unique_ptr<InputHandler>(new InputHandler());
    
    mServiceLocator->RegisterService<EntityComponentManager>(mEntityComponentManager.get());
    mServiceLocator->RegisterService<EventCommunicationService>(mEventCommunicationService.get());
    mServiceLocator->RegisterService<CoreRenderingService>(mCoreRenderingService.get());
    mServiceLocator->RegisterService<PhysicsSystem>(mPhysicsSystem.get());
    mServiceLocator->RegisterService<ResourceManager>(mResourceManager.get());
    mServiceLocator->RegisterService<InputHandler>(mInputHandler.get());
    
    if (!mCoreRenderingService->InitializeEngine()) return false;
    if (!mResourceManager->InitializeResourceLoaders()) return false;
    
    auto playerTextureResourceId = mResourceManager->LoadResource("player.png");
    auto backgroundTextureResourceId = mResourceManager->LoadResource("jungle-sky.png");
    auto groundTextureResourceId = mResourceManager->LoadResource("environments/jungle_tiles/ground_top_middle.png");
    
    auto playerTextureId = mResourceManager->GetResource<TextureResource>(playerTextureResourceId).GetGLTextureId();
    auto backgroundTextureId = mResourceManager->GetResource<TextureResource>(backgroundTextureResourceId).GetGLTextureId();    
    auto groundTextureId = mResourceManager->GetResource<TextureResource>(groundTextureResourceId).GetGLTextureId();
                                                         
    mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());
    
    auto backgroundTransformComponent = std::make_unique<TransformationComponent>();
    auto backgroundAnimationComponent = std::make_unique<AnimationComponent>(std::vector<GLuint>{backgroundTextureId});
    auto backgroundShaderComponent = std::make_unique<ShaderComponent>("background");
    
    mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds[0], std::move(backgroundTransformComponent));
    mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds[0], std::move(backgroundAnimationComponent));
    mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds[0], std::move(backgroundShaderComponent));
    
    mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());
    
    auto playerTransformComponent = std::make_unique<TransformationComponent>();
    auto playerAnimationComponent = std::make_unique<AnimationComponent>(std::vector<GLuint>{playerTextureId});
    auto playerShaderComponent = std::make_unique<ShaderComponent>("basic");
    auto playerPhysicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::DYNAMIC);
    
    playerTransformComponent->GetScale() = glm::vec3(80.0f, 80.0f, 1.0f);
	playerTransformComponent->GetTranslation() = glm::vec3(640.0f, 360.0f, 10.0f);
    playerPhysicsComponent->GetGravity() = glm::vec3(0.0f, -1680.0f, 0.0f);
    playerPhysicsComponent->GetMinVelocity() = glm::vec3(-240.0f, -480.0f, 0.0f);
    playerPhysicsComponent->GetMaxVelocity() = glm::vec3(240.0f, 480.0f, 0.0f);
                                                                     
    mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds[1], std::move(playerTransformComponent));
    mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds[1], std::move(playerAnimationComponent));
    mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds[1], std::move(playerShaderComponent));
    mEntityComponentManager->AddComponent<PhysicsComponent>(mActiveEntityIds[1], std::move(playerPhysicsComponent));
    
    mPlayerController = std::make_unique<PlayerController>(*mEntityComponentManager, mActiveEntityIds[1]);
    
    for (int i = 0; i < 10; ++i)
    {
        mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());
        auto transformationComponent = std::make_unique<TransformationComponent>();
        transformationComponent->GetScale() = glm::vec3(80.0f, 80.0f, 1.0f);
        transformationComponent->GetTranslation() = glm::vec3(40.0f + static_cast<float>(i) * 80.0f, 40.0f, 1.0f);
        auto shaderComponent = std::make_unique<ShaderComponent>("basic");
        auto physicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::STATIC);
        auto animationComponent = std::make_unique<AnimationComponent>(std::vector<GLuint>{groundTextureId});
        
        mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds.back(), std::move(transformationComponent));
        mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds.back(), std::move(animationComponent));
        mEntityComponentManager->AddComponent<PhysicsComponent>(mActiveEntityIds.back(), std::move(physicsComponent));
        mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds.back(), std::move(shaderComponent));
    }
    
    return true;
}


void App::Update(const float dt)
{
    HandleInput();
    mPhysicsSystem->UpdateEntities(mActiveEntityIds, dt);
    mCoreRenderingService->RenderEntities(mActiveEntityIds);
}

void App::HandleInput()
{
    const auto inputActions = mInputHandler->TranslateInputToActions();
    for (const auto inputAction: inputActions)
    {
        mPlayerController->ConsumeInputAction(inputAction);
    }
}

