//
//  App.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "App.h"
#include "game/AIService.h"
#include "game/PlayerAttackRechargeController.h"
#include "rendering/Camera.h"
#include "input/PlayerInputActionConsumer.h"
#include "input/DebugInputActionConsumer.h"
#include "components/EntityComponentManager.h"
#include "components/IAIComponent.h"
#include "events/EventCommunicator.h"
#include "events/AnnouncePlayerEntityIdEvent.h"
#include "events/EntityDamagedEvent.h"
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
#include "game/Level.h"
#include "game/LevelFactory.h"
#include "game/DamageSystem.h"
#include "util/TypeTraits.h"
#include "util/StringId.h"

#include <vector>


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
    // Initialize services
    mServiceLocator = std::unique_ptr<ServiceLocator>(new ServiceLocator);
    mAIService = std::unique_ptr<AIService>(new AIService(*mServiceLocator));
    mEntityComponentManager = std::unique_ptr<EntityComponentManager>(new EntityComponentManager);
    mEventCommunicationService = std::unique_ptr<EventCommunicationService>(new EventCommunicationService);
    mResourceManager = std::unique_ptr<ResourceManager>(new ResourceManager());
    mCoreRenderingService = std::unique_ptr<CoreRenderingService>(new CoreRenderingService(*mServiceLocator));
    mAnimationService = std::unique_ptr<AnimationService>(new AnimationService(*mServiceLocator));
    mPhysicsSystem = std::unique_ptr<PhysicsSystem>(new PhysicsSystem(*mServiceLocator));
    mDamageSystem = std::unique_ptr<DamageSystem>(new DamageSystem(*mServiceLocator));
    mInputHandler = std::unique_ptr<InputHandler>(new InputHandler());
    mPlayerAttackRechargeController = std::unique_ptr<PlayerAttackRechargeController>(new PlayerAttackRechargeController(*mServiceLocator));
    
    // Register services
    mServiceLocator->RegisterService<EntityComponentManager>(mEntityComponentManager.get());
    mServiceLocator->RegisterService<EventCommunicationService>(mEventCommunicationService.get());
    mServiceLocator->RegisterService<CoreRenderingService>(mCoreRenderingService.get());
    mServiceLocator->RegisterService<PhysicsSystem>(mPhysicsSystem.get());
    mServiceLocator->RegisterService<ResourceManager>(mResourceManager.get());
    mServiceLocator->RegisterService<InputHandler>(mInputHandler.get());
    mServiceLocator->RegisterService<PlayerAttackRechargeController>(mPlayerAttackRechargeController.get());

    // 2nd step service initialization
    if (!mDamageSystem->VInitialize()) return false;
    if (!mPhysicsSystem->VInitialize()) return false;
    if (!mAIService->VInitialize()) return false;
    if (!mCoreRenderingService->VInitialize()) return false;
    if (!mResourceManager->VInitialize()) return false;
    if (!mPlayerAttackRechargeController->VInitialize()) return false;
    
    // Parse Level
    LevelFactory levelFactory(*mServiceLocator);
    mLevel = levelFactory.CreateLevel("editornew.json");

    // Initialize camera
    mCamera = std::make_unique<Camera>(*mServiceLocator, mCoreRenderingService->GetRenderableDimensions(), mLevel->GetHorizontalBounds(), mLevel->GetVerticalBounds());
    mCoreRenderingService->AttachCamera(mCamera.get());
    
    // Initialized in order of priority
    mInputActionConsumers.push_back(std::make_unique<DebugInputActionConsumer>(*mServiceLocator));
    mInputActionConsumers.push_back(std::make_unique<PlayerInputActionConsumer>(*mServiceLocator, mLevel->GetEntityIdFromName(StringId("char-player"))));
    
    // Announce player id to AIs
    mEventCommunicator = mEventCommunicationService->CreateEventCommunicator();
    mEventCommunicator->DispatchEvent(std::make_unique<AnnouncePlayerEntityIdEvent>(mLevel->GetEntityIdFromName(StringId("char-player"))));
    mEventCommunicator->RegisterEventCallback<EntityDamagedEvent>([](const IEvent& event)
    {
        const auto& actualEvent = static_cast<const EntityDamagedEvent&>(event);
        Log(LogType::INFO, "Entity %d damaged %.2f, now has %.2f health", static_cast<int>(actualEvent.GetDamagedEntityId()), actualEvent.GetDamageDone(), actualEvent.GetHealthRemaining());
    });
    
    return true;
}


void App::Update(const float dt)
{    
    mLevel->CheckForAdditionsOrRemovalsOfEntities();
    HandleInput();
    mPlayerAttackRechargeController->Update(dt);
    mAIService->UpdateAIComponents(mLevel->GetAllActiveEntities(), dt);
    mDamageSystem->Update(mLevel->GetAllActiveEntities(), dt);
    mPhysicsSystem->UpdateEntities(mLevel->GetAllActiveEntities(), dt);
    mAnimationService->UpdateAnimations(mLevel->GetAllActiveEntities(), dt);
    mCamera->Update(mLevel->GetEntityIdFromName(StringId("char-player")), dt);
    mCoreRenderingService->RenderEntities(mLevel->GetAllActiveEntities());
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

