//
//  App.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "App.h"
#include "input/PlayerInputActionConsumer.h"
#include "input/DebugInputActionConsumer.h"
#include "components/EntityComponentManager.h"
#include "components/TransformationComponent.h"
#include "components/AnimationComponent.h"
#include "components/ShaderComponent.h"
#include "components/PhysicsComponent.h"
#include "events/EventCommunicationService.h"
#include "rendering/CoreRenderingService.h"
#include "rendering/AnimationService.h"
#include "physics/PhysicsSystem.h"
#include "ServiceLocator.h"
#include "resources/ResourceManager.h"
#include "util/Logging.h"
#include "util/FileUtils.h"
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
    mAnimationService = std::unique_ptr<AnimationService>(new AnimationService(*mServiceLocator));
    mPhysicsSystem = std::unique_ptr<PhysicsSystem>(new PhysicsSystem(*mServiceLocator));
    mInputHandler = std::unique_ptr<InputHandler>(new InputHandler());

    mServiceLocator->RegisterService<EntityComponentManager>(mEntityComponentManager.get());
    mServiceLocator->RegisterService<EventCommunicationService>(mEventCommunicationService.get());
    mServiceLocator->RegisterService<CoreRenderingService>(mCoreRenderingService.get());
    mServiceLocator->RegisterService<PhysicsSystem>(mPhysicsSystem.get());
    mServiceLocator->RegisterService<ResourceManager>(mResourceManager.get());
    mServiceLocator->RegisterService<InputHandler>(mInputHandler.get());

    mPhysicsSystem->Initialize();
    if (!mCoreRenderingService->InitializeEngine()) return false;
    if (!mResourceManager->InitializeResourceLoaders()) return false;


    auto backgroundTextureResourceId = mResourceManager->LoadResource("jungle-sky.png");
    auto groundTextureResourceId = mResourceManager->LoadResource("environments/jungle_tiles/ground_top_middle.png");
    mResourceManager->LoadResource("debug/debug_square.png");

    //auto groundLeftTextureResourceId = mResourceManager->LoadResource("environments/jungle_tiles/ground_top_edge_left_side.png");
    //auto groundRightTextureResourceId = mResourceManager->LoadResource("environments/jungle_tiles/ground_top_edge_right_side.png");

    auto backgroundTextureId = mResourceManager->GetResource<TextureResource>(backgroundTextureResourceId).GetGLTextureId();
    auto groundTextureId = mResourceManager->GetResource<TextureResource>(groundTextureResourceId).GetGLTextureId();

    //auto groundLeftTextureId = mResourceManager->GetResource<TextureResource>(groundLeftTextureResourceId).GetGLTextureId();
    //auto groundRightTextureId = mResourceManager->GetResource<TextureResource>(groundRightTextureResourceId).GetGLTextureId();    

    {
        mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());

        auto backgroundTransformComponent = std::make_unique<TransformationComponent>();
        backgroundTransformComponent->GetScale() *= 2.0f;

        auto backgroundShaderComponent = std::make_unique<ShaderComponent>("background");
        mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds.back(), std::move(backgroundTransformComponent));
        std::map<StringId, std::vector<GLuint>> backgroundAnimations = { { StringId("background"),{ backgroundTextureId } } };
        auto backgroundAnimationComponent = std::make_unique<AnimationComponent>(backgroundAnimations, 100.0f);
        mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds.back(), std::move(backgroundAnimationComponent));
        mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds.back(), std::move(backgroundShaderComponent));
    }
    

    mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());
    auto movingPlatformTransformationComponent = std::make_unique<TransformationComponent>();
    movingPlatformTransformationComponent->GetScale() = glm::vec3(80.0f, 80.0f, 1.0f);
    movingPlatformTransformationComponent->GetTranslation() = glm::vec3(900.0f, 100.0f, 1.0f);
    auto movingPlatformShaderComponent = std::make_unique<ShaderComponent>("basic");
    auto movingPlatformPhysicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::KINEMATIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(80.0f, 80.0f)));

    std::map<StringId, std::vector<GLuint>> movingPlatformAnimations = { { StringId("ground"),{ groundTextureId } } };
    auto movingPlatformAnimationComponent = std::make_unique<AnimationComponent>(movingPlatformAnimations, 100.0f);

    mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds.back(), std::move(movingPlatformTransformationComponent));
    mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds.back(), std::move(movingPlatformAnimationComponent));
    mEntityComponentManager->AddComponent<PhysicsComponent>(mActiveEntityIds.back(), std::move(movingPlatformPhysicsComponent));
    mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds.back(), std::move(movingPlatformShaderComponent));

    {
    mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());
    
    auto playerTransformComponent = std::make_unique<TransformationComponent>();
    std::map<StringId, std::vector<GLuint>> playerAnimations;
    
    auto idleAnimationFileNames = GetAllFilenamesInDirectory(mResourceManager->GetRootResourceDirectory() + "characters/player/idle");
    for (const auto fileName : idleAnimationFileNames)
    {
        auto resourceId = mResourceManager->LoadResource("characters/player/idle/" + fileName);
        playerAnimations[StringId("idle")].push_back(mResourceManager->GetResource<TextureResource>(resourceId).GetGLTextureId());
    }

    auto runAnimationFileNames = GetAllFilenamesInDirectory(mResourceManager->GetRootResourceDirectory() + "characters/player/running");
    for (const auto fileName : runAnimationFileNames)
    {
        auto resourceId = mResourceManager->LoadResource("characters/player/running/" + fileName);
        playerAnimations[StringId("running")].push_back(mResourceManager->GetResource<TextureResource>(resourceId).GetGLTextureId());
    }    
  
    auto playerAnimationComponent = std::make_unique<AnimationComponent>(playerAnimations, 0.05f);
    playerAnimationComponent->ChangeAnimation(StringId("idle"));

    auto playerShaderComponent = std::make_unique<ShaderComponent>("basic");
    auto playerPhysicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::DYNAMIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(60.0f, 90.0f)));
    
    playerTransformComponent->GetScale() = glm::vec3(160.0f, 160.0f, 1.0f);
	playerTransformComponent->GetTranslation() = glm::vec3(640.0f, 800.0f, 1.0f);
    playerPhysicsComponent->GetGravity() = glm::vec3(0.0f, -1680.0f, 0.0f);
    playerPhysicsComponent->GetMinVelocity() = glm::vec3(-240.0f, -480.0f, 0.0f);
    playerPhysicsComponent->GetMaxVelocity() = glm::vec3(240.0f, 480.0f, 0.0f);
                                                                     
    mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds.back(), std::move(playerTransformComponent));
    mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds.back(), std::move(playerAnimationComponent));
    mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds.back(), std::move(playerShaderComponent));
    mEntityComponentManager->AddComponent<PhysicsComponent>(mActiveEntityIds.back(), std::move(playerPhysicsComponent));
    }
       
    /*
    for (int i = 1; i < 2; ++i)
    {
        mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());
        auto transformationComponent = std::make_unique<TransformationComponent>();
        transformationComponent->GetScale() = glm::vec3(80.0f, 80.0f, 1.0f);
        transformationComponent->GetTranslation() = glm::vec3(80.0f, 80.0f, 1.0f);
        auto shaderComponent = std::make_unique<ShaderComponent>("basic");
        auto physicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::STATIC);
        
        std::map<StringId, std::vector<GLuint>> groundTextureAnimations = {{ StringId("ground_left"), {groundLeftTextureId} }};
        auto animationComponent = std::make_unique<AnimationComponent>(groundTextureAnimations, 100.0f);
        
        mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds.back(), std::move(transformationComponent));
        mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds.back(), std::move(animationComponent));
        mEntityComponentManager->AddComponent<PhysicsComponent>(mActiveEntityIds.back(), std::move(physicsComponent));
        mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds.back(), std::move(shaderComponent));
    }
   */
    for (int i = 5; i < 10; ++i)
    {
        mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());
        auto transformationComponent = std::make_unique<TransformationComponent>();
        transformationComponent->GetScale() = glm::vec3(80.0f, 80.0f, 1.0f);
        transformationComponent->GetTranslation() = glm::vec3(40.0f + static_cast<float>(i) * 80.0f, 40.0f, 1.0f);
        auto shaderComponent = std::make_unique<ShaderComponent>("basic");
        auto physicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::STATIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(80.0f, 80.0f)));

        std::map<StringId, std::vector<GLuint>> groundTextureAnimations = {{ StringId("ground"), {groundTextureId} }};
        auto animationComponent = std::make_unique<AnimationComponent>(groundTextureAnimations, 100.0f);
        
        mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds.back(), std::move(transformationComponent));
        mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds.back(), std::move(animationComponent));
        mEntityComponentManager->AddComponent<PhysicsComponent>(mActiveEntityIds.back(), std::move(physicsComponent));
        mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds.back(), std::move(shaderComponent));
    }
    
    for (int i = 5; i < 10; ++i)
    {
        mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());
        auto transformationComponent = std::make_unique<TransformationComponent>();
        transformationComponent->GetScale() = glm::vec3(80.0f, 80.0f, 1.0f);
        transformationComponent->GetTranslation() = glm::vec3(40.0f + static_cast<float>(i) * 80.0f, 460.0f, 1.0f);
        auto shaderComponent = std::make_unique<ShaderComponent>("basic");
        auto physicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::STATIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(80.0f, 80.0f)));
        
        std::map<StringId, std::vector<GLuint>> groundTextureAnimations = {{ StringId("ground"), {groundTextureId} }};
        auto animationComponent = std::make_unique<AnimationComponent>(groundTextureAnimations, 100.0f);
        
        mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds.back(), std::move(transformationComponent));
        mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds.back(), std::move(animationComponent));
        mEntityComponentManager->AddComponent<PhysicsComponent>(mActiveEntityIds.back(), std::move(physicsComponent));
        mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds.back(), std::move(shaderComponent));
    }
    
    for (int i = 0; i < 5; ++i)
    {
        mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());
        auto transformationComponent = std::make_unique<TransformationComponent>();
        transformationComponent->GetScale() = glm::vec3(80.0f, 80.0f, 1.0f);
        transformationComponent->GetTranslation() = glm::vec3(440, 120.0f + static_cast<float>(i) * 80.0f, 1.0f);
        auto shaderComponent = std::make_unique<ShaderComponent>("basic");
        auto physicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::STATIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(80.0f, 80.0f)));
        
        std::map<StringId, std::vector<GLuint>> groundTextureAnimations = {{ StringId("ground"), {groundTextureId} }};
        auto animationComponent = std::make_unique<AnimationComponent>(groundTextureAnimations, 100.0f);
        
        mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds.back(), std::move(transformationComponent));
        mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds.back(), std::move(animationComponent));
        mEntityComponentManager->AddComponent<PhysicsComponent>(mActiveEntityIds.back(), std::move(physicsComponent));
        mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds.back(), std::move(shaderComponent));
    }       

    /*
    for (int i = 1; i < 2; ++i)
    {
        mActiveEntityIds.push_back(mEntityComponentManager->GenerateEntity());
        auto transformationComponent = std::make_unique<TransformationComponent>();
        transformationComponent->GetScale() = glm::vec3(80.0f, 80.0f, 1.0f);
        transformationComponent->GetTranslation() = glm::vec3(840.0f, 40.0f, 1.0f);
        auto shaderComponent = std::make_unique<ShaderComponent>("basic");
        auto physicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::STATIC);
        
        std::map<StringId, std::vector<GLuint>> groundTextureAnimations = {{ StringId("ground_right"), {groundRightTextureId} }};
        auto animationComponent = std::make_unique<AnimationComponent>(groundTextureAnimations, 100.0f);
        
        mEntityComponentManager->AddComponent<TransformationComponent>(mActiveEntityIds.back(), std::move(transformationComponent));
        mEntityComponentManager->AddComponent<AnimationComponent>(mActiveEntityIds.back(), std::move(animationComponent));
        mEntityComponentManager->AddComponent<PhysicsComponent>(mActiveEntityIds.back(), std::move(physicsComponent));
        mEntityComponentManager->AddComponent<ShaderComponent>(mActiveEntityIds.back(), std::move(shaderComponent));
    }
    */
    
    // Initialized in order of priority
    mInputActionConsumers.push_back(std::make_unique<DebugInputActionConsumer>(*mServiceLocator));
    mInputActionConsumers.push_back(std::make_unique<PlayerInputActionConsumer>(*mEntityComponentManager, mActiveEntityIds[2]));
    
    return true;
}


void App::Update(const float dt)
{
    HandleInput();
    
    static float timeElapsed = 0.0f;
    static bool goingRight = true;

    timeElapsed += dt;
    if (timeElapsed > 5.0f)
    {
        timeElapsed = 0.0f;
        goingRight = !goingRight;    
    }    

    auto& physicsComponent = mEntityComponentManager->GetComponent<PhysicsComponent>(mActiveEntityIds[1]);
    //physicsComponent.GetVelocity().x = goingRight ? 60.0f : -60.0f;
    physicsComponent.GetVelocity().y = goingRight ? 60.0f : -60.0f;

    mPhysicsSystem->UpdateEntities(mActiveEntityIds, dt);
    mAnimationService->UpdateAnimations(mActiveEntityIds, dt);
    mCoreRenderingService->RenderEntities(mActiveEntityIds);
}

void App::HandleInput()
{
    const auto inputActions = mInputHandler->TranslateInputToActions();
    for (const auto inputAction: inputActions)
    {
        for (const auto& inputActionConsumer: mInputActionConsumers)
        {
            if (inputActionConsumer->VConsumeInputAction(inputAction))
                break;
        }
    }
}

