//
//  App.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "App.h"
#include "game/AIService.h"
#include "game/PlayerBehaviorController.h"
#include "rendering/Camera.h"
#include "input/PlayerInputActionConsumer.h"
#include "input/DebugInputActionConsumer.h"
#include "commands/SetVelocityAndAnimateCommand.h"
#include "components/AnimationComponent.h"
#include "components/EntityComponentManager.h"
#include "components/IAIComponent.h"
#include "components/ShaderComponent.h"
#include "events/EventCommunicator.h"
#include "events/AnnouncePlayerEntityIdEvent.h"
#include "events/EntityDamagedEvent.h"
#include "events/PlayerRespawnEvent.h"
#include "events/LevelCreatedEvent.h"
#include "rendering/CoreRenderingService.h"
#include "rendering/AnimationService.h"
#include "rendering/effects/EffectsManager.h"
#include "physics/PhysicsSystem.h"
#include "ServiceLocator.h"
#include "resources/ResourceManager.h"
#include "util/Logging.h"
#include "util/FileUtils.h"
#include "gl/Context.h"
#include "resources/TextureResource.h"
#include "input/InputHandler.h"
#include "game/Level.h"
#include "game/LevelFactory.h"
#include "game/DamageSystem.h"
#include "game/ui/UIElementManager.h"
#include "util/TypeTraits.h"
#include "util/StringId.h"

#include <vector>
#include <chrono>

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
    if (!InitializeServices()) return false;
    if (!InitializeGame()) return false;
    return true;
}

void App::Update(const float dt)
{
    if (mShouldRestartLevelOnPlayerDeath)
    {
        InitializeGame();
    }
    
    mLevel->CheckForAdditionsOrRemovalsOfEntities();
    HandleInput();
    mPlayerBehaviorController->Update(dt);
    mAIService->UpdateAIComponents(mLevel->GetAllActiveEntities(), dt);
    mDamageSystem->Update(mLevel->GetAllActiveEntities(), dt);
    mUIElementManager->Update(dt);
    mEffectsManager->Update(dt);
    mAnimationService->UpdateAnimations(mLevel->GetAllActiveEntities(), dt);
    mCamera->Update(mLevel->GetEntityIdFromName(StringId("player")), dt);
    
#ifndef NDEBUG
    const auto prePhysicsTimeStamp = std::chrono::high_resolution_clock::now();
#endif
    
    mPhysicsSystem->UpdateEntities(mLevel->GetAllActiveEntities(), dt);
    
#ifndef NDEBUG
    const auto postPhysicsTimeStamp = std::chrono::high_resolution_clock::now();
#endif
    
    mCoreRenderingService->RenderEntities(mLevel->GetAllActiveEntities());
    mCoreRenderingService->RenderEntities(mUIElementManager->GetUIEntities());
    
#ifndef NDEBUG
    const auto postRenderingTimeStamp = std::chrono::high_resolution_clock::now();
    const auto elapsedPhysicsSeconds = postPhysicsTimeStamp - prePhysicsTimeStamp;
    const auto elapsedRenderingSeconds = postRenderingTimeStamp - postPhysicsTimeStamp;
    mCoreRenderingService->SetFrameStatisticsMessage("  |  Physics: " + (std::to_string(elapsedPhysicsSeconds.count() * 1e-6)) +
                                                   "ms  |  " + "Rendering: " + (std::to_string(elapsedRenderingSeconds.count() * 1e-6)) + "ms");
#endif
}

void App::HandleInput()
{
    const auto inputActions = mInputHandler->TranslateInputToActions();
    for (const auto inputAction: inputActions)
    {
        for (const auto& inputActionConsumer: mInputActionConsumers)
        {
            if (inputActionConsumer->VConsumeInputAction(inputAction)) break;
        }
    }
}

bool App::InitializeServices()
{
    // Initialize services
    mServiceLocator = std::unique_ptr<ServiceLocator>(new ServiceLocator);
    mAIService = std::unique_ptr<AIService>(new AIService(*mServiceLocator));
    mEntityComponentManager = std::unique_ptr<EntityComponentManager>(new EntityComponentManager);
    mEventCommunicationService = std::unique_ptr<EventCommunicationService>(new EventCommunicationService);
    mResourceManager = std::unique_ptr<ResourceManager>(new ResourceManager());
    mCoreRenderingService = std::unique_ptr<CoreRenderingService>(new CoreRenderingService(*mServiceLocator));
    mAnimationService = std::unique_ptr<AnimationService>(new AnimationService(*mServiceLocator));
    mPhysicsSystem = std::unique_ptr<PhysicsSystem>(new PhysicsSystem(*mServiceLocator));
    mUIElementManager = std::unique_ptr<UIElementManager>(new UIElementManager(*mServiceLocator));
    mEffectsManager = std::unique_ptr<EffectsManager>(new EffectsManager(*mServiceLocator));
    mDamageSystem = std::unique_ptr<DamageSystem>(new DamageSystem(*mServiceLocator));
    mInputHandler = std::unique_ptr<InputHandler>(new InputHandler());
    mPlayerBehaviorController = std::unique_ptr<PlayerBehaviorController>(new PlayerBehaviorController(*mServiceLocator));
    
    // Register services
    mServiceLocator->RegisterService<EntityComponentManager>(mEntityComponentManager.get());
    mServiceLocator->RegisterService<EventCommunicationService>(mEventCommunicationService.get());
    mServiceLocator->RegisterService<CoreRenderingService>(mCoreRenderingService.get());
    mServiceLocator->RegisterService<PhysicsSystem>(mPhysicsSystem.get());
    mServiceLocator->RegisterService<UIElementManager>(mUIElementManager.get());
    mServiceLocator->RegisterService<EffectsManager>(mEffectsManager.get());
    mServiceLocator->RegisterService<ResourceManager>(mResourceManager.get());
    mServiceLocator->RegisterService<InputHandler>(mInputHandler.get());
    mServiceLocator->RegisterService<PlayerBehaviorController>(mPlayerBehaviorController.get());
    
    // 2nd step service initialization
    if (!mDamageSystem->VInitialize()) return false;
    if (!mPhysicsSystem->VInitialize()) return false;
    if (!mUIElementManager->VInitialize()) return false;
    if (!mEffectsManager->VInitialize()) return false;
    if (!mAIService->VInitialize()) return false;
    if (!mCoreRenderingService->VInitialize()) return false;
    if (!mResourceManager->VInitialize()) return false;
    if (!mPlayerBehaviorController->VInitialize()) return false;
    
    mUIElementManager->InitializeUIElements();
    
    return true;
}

bool App::InitializeGame()
{
    mShouldRestartLevelOnPlayerDeath = false;
    
    // Parse Level
    LevelFactory levelFactory(*mServiceLocator);
    mLevel = levelFactory.CreateLevel("editornew.json");
    
    // Initialize camera
    mCamera = std::make_unique<Camera>(*mServiceLocator, mCoreRenderingService->GetRenderableDimensions(), mLevel->GetHorizontalBounds(), mLevel->GetVerticalBounds());
    mCoreRenderingService->AttachCamera(mCamera.get());
    
    // Initialized in order of priority
    mInputActionConsumers.clear();
    mInputActionConsumers.emplace_back(std::make_unique<DebugInputActionConsumer>(*mServiceLocator));
    mInputActionConsumers.emplace_back(std::make_unique<PlayerInputActionConsumer>(*mServiceLocator, mLevel->GetEntityIdFromName(StringId("player"))));
    
    // Announce player id
    mEventCommunicator = mEventCommunicationService->CreateEventCommunicator();
    
    // Dispatch game related events
    mEventCommunicator->DispatchEvent(std::make_unique<AnnouncePlayerEntityIdEvent>(mLevel->GetEntityIdFromName(StringId("player"))));
    mEventCommunicator->DispatchEvent(std::make_unique<LevelCreatedEvent>(mLevel->GetHorizontalBounds().y, mLevel->GetVerticalBounds().y));
    
    // Listen to game related events
    mEventCommunicator->RegisterEventCallback<EntityDamagedEvent>([this](const IEvent& event)
    {
        const auto& actualEvent = static_cast<const EntityDamagedEvent&>(event);
        const auto damagedEntityName = mLevel->GetEntityNameFromId(actualEvent.GetDamagedEntityId()).GetString();
        const auto damageSenderEntityName = mLevel->GetEntityNameFromId(actualEvent.GetDamageSenderEntityId()).GetString();
        
        Log(LogType::INFO, "Entity %d (%s) damaged %.2f by entity %d (%s), now has %.2f health",
            static_cast<int>(actualEvent.GetDamagedEntityId()),
            damagedEntityName.c_str(),
            actualEvent.GetDamageDone(),
            static_cast<int>(actualEvent.GetDamageSenderEntityId()),
            damageSenderEntityName.c_str(),
            actualEvent.GetHealthRemaining());
    });
    mEventCommunicator->RegisterEventCallback<PlayerRespawnEvent>([this](const IEvent&)
    {
        mShouldRestartLevelOnPlayerDeath = true;
    });
    
    // Temp place. Add animation displacements
    mEntityComponentManager->GetComponent<AnimationComponent>(mLevel->GetEntityIdFromName(StringId("player"))).SetSpecificAnimationDisplacement(StringId("flamebreath"), glm::vec2(100.0f, 20.0f));
    mEntityComponentManager->GetComponent<AnimationComponent>(mLevel->GetEntityIdFromName(StringId("player"))).SetSpecificAnimationDisplacement(StringId("melee"), glm::vec2(13.0f, 8.0f));
    mEntityComponentManager->GetComponent<AnimationComponent>(mLevel->GetEntityIdFromName(StringId("player"))).SetSpecificAnimationFrameDuration(StringId("flamebreath"), 0.06f);
    return true;
}
