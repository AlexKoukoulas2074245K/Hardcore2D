//
//  EffectsManager.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/02/2019.
//

#include "EffectsManager.h"
#include "../../ServiceLocator.h"
#include "../../events/EventCommunicator.h"
#include "../../events/NewEntityCreatedEvent.h"
#include "../../events/EntityDestroyedEvent.h"
#include "../../events/PlayerKilledEvent.h"
#include "../../events/PlayerRespawnEvent.h"
#include "../../commands/SetVelocityCommand.h"
#include "../../components/EntityComponentManager.h"
#include "../../components/ShaderComponent.h"
#include "../../components/AnimationComponent.h"
#include "../../components/PhysicsComponent.h"
#include "../../components/TransformComponent.h"
#include "../../components/BloodDropAIComponent.h"
#include "../../components/FactionComponent.h"
#include "../../resources/ResourceManager.h"
#include "../../resources/TextureResource.h"
#include "../CoreRenderingService.h"
#include "../../util/MathUtils.h"

const float EffectsManager::MAX_BLUR_UPON_DEATH = 10.0f;
const float EffectsManager::BLUR_SPEED_UPON_DEATH = 2.0f;

EffectsManager::EffectsManager(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
    , mSceneBlurIntensity(0.0f)
    , mIsBlurIntensifying(false)
    , mDeathQuoteEntityId(EntityComponentManager::NULL_ENTITY_ID)
    , mRespawnQuoteEntityId(EntityComponentManager::NULL_ENTITY_ID)
{
}

EffectsManager::~EffectsManager()
{
}

bool EffectsManager::VInitialize()
{
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
    mResourceManager = &(mServiceLocator.ResolveService<ResourceManager>());
    mCoreRenderingService = &(mServiceLocator.ResolveService<CoreRenderingService>());
    mEventCommunicator = mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    
    RegisterEventCallbacks();

    return true;
}

void EffectsManager::Update(const float dt)
{
    mSceneBlurIntensity += (mIsBlurIntensifying ? BLUR_SPEED_UPON_DEATH : -5.0f * BLUR_SPEED_UPON_DEATH) * dt;

    if (mIsBlurIntensifying == false && mSceneBlurIntensity <= 0.0f)
    {
        mSceneBlurIntensity = 0.0f;
    }
    else if (mIsBlurIntensifying && mSceneBlurIntensity >= MAX_BLUR_UPON_DEATH)
    {
        mSceneBlurIntensity = MAX_BLUR_UPON_DEATH;
    }

    mCoreRenderingService->SetSceneBlurIntensity(mSceneBlurIntensity);
}

void EffectsManager::PlayEffect(const glm::vec3& effectOrigin, const EffectType effectType)
{
    switch (effectType)
    {
        case EffectType::BLOOD_SPURT_SINGLE: CreateBloodSpurtSingleEffect(effectOrigin); break;
        case EffectType::BLOOD_SPURT_MULTI: CreateBloodSpurtMultiEffect(effectOrigin); break;
    }
}

void EffectsManager::CreateBloodSpurtMultiEffect(const glm::vec3& effectOrigin)
{
    const auto bloodDropsCount = RandomInt(6, 12);
    
    for (int i = 0; i < bloodDropsCount; ++i)
    {
        CreateBloodSpurtSingleEffect(effectOrigin);
    }
}

void EffectsManager::CreateBloodSpurtSingleEffect(const glm::vec3& effectOrigin)
{
    const auto bloodDropEntityId = mEntityComponentManager->GenerateEntity();
    mEntityComponentManager->AddComponent<ShaderComponent>(bloodDropEntityId, std::make_unique<ShaderComponent>(StringId("basic")));
    
    const auto& frameTextureResource = mResourceManager->GetResource<TextureResource>("effects/blood_drop/idle/0.png");
    AnimationComponent::AnimationsMap mAnimation;
    mAnimation[StringId("idle")].push_back(frameTextureResource.GetGLTextureId());
    mEntityComponentManager->AddComponent<AnimationComponent>(bloodDropEntityId, std::make_unique<AnimationComponent>(mAnimation, 100.0f));
    
    auto bloodDropPhysicsComponent = std::make_unique<PhysicsComponent>
    (PhysicsComponent::BodyType::DYNAMIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)), glm::vec3(0.0f, -1680.0f, 0.0f), glm::vec3(500.0f, 500.0f, 0.0f), glm::vec3(-500.0f, -500.0f, 0.0f));
    
    const auto randomVelocityX = RandomFloat(-200.0f, 200.0f);
    const auto randomVelocityY = RandomFloat(0.0f, 200.0f);
    
    mEntityComponentManager->AddComponent<PhysicsComponent>(bloodDropEntityId, std::move(bloodDropPhysicsComponent));
    SetVelocityCommand(*mEntityComponentManager, bloodDropEntityId, glm::vec3(randomVelocityX, randomVelocityY, 0.0f)).VExecute();
    
    auto bloodDropTransformComponent = std::make_unique<TransformComponent>();
    bloodDropTransformComponent->GetTranslation().x = effectOrigin.x;
    bloodDropTransformComponent->GetTranslation().y = effectOrigin.y;
    bloodDropTransformComponent->GetScale() = glm::vec3(8.0f, 8.0f, 1.0f);
    
    mEntityComponentManager->AddComponent<FactionComponent>(bloodDropEntityId, std::make_unique<FactionComponent>(FactionGroup::ALLIES));
    mEntityComponentManager->AddComponent<TransformComponent>(bloodDropEntityId, std::move(bloodDropTransformComponent));
    mEntityComponentManager->AddComponent<IAIComponent>(bloodDropEntityId, std::make_unique<BloodDropAIComponent>(mServiceLocator, bloodDropEntityId, 0.5f));
    mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(EntityNameIdEntry(StringId("blood_drop"), bloodDropEntityId)));
}

void EffectsManager::RegisterEventCallbacks()
{
    mEventCommunicator->RegisterEventCallback<PlayerKilledEvent>([this](const IEvent&) 
    {
        mIsBlurIntensifying = true;
        CreateDeathQuoteEntity();
    });

    mEventCommunicator->RegisterEventCallback<PlayerRespawnEvent>([this](const IEvent&) 
    {
        mIsBlurIntensifying = false;
        if (mDeathQuoteEntityId != EntityComponentManager::NULL_ENTITY_ID)
        {
            mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mDeathQuoteEntityId));
            mDeathQuoteEntityId = EntityComponentManager::NULL_ENTITY_ID;
        }

        if (mRespawnQuoteEntityId != EntityComponentManager::NULL_ENTITY_ID)
        {
            mEventCommunicator->DispatchEvent(std::make_unique<EntityDestroyedEvent>(mRespawnQuoteEntityId));
            mRespawnQuoteEntityId = EntityComponentManager::NULL_ENTITY_ID;
        }
    });
}

void EffectsManager::CreateDeathQuoteEntity()
{
    mDeathQuoteEntityId = mEntityComponentManager->GenerateEntity();
    mEntityComponentManager->AddComponent<ShaderComponent>(mDeathQuoteEntityId, std::make_unique<ShaderComponent>(StringId("background"), false));
    
    const auto deathQuoteNumber = RandomInt(0, 2);    
    mEntityComponentManager->AddComponent<AnimationComponent>(mDeathQuoteEntityId, std::make_unique<AnimationComponent>("misc/death_quote_" + std::to_string(deathQuoteNumber), 0.05f, *mResourceManager, false, false, [this]() 
    {
        CreateRespawnQuoteEntity();
    }));

    mEntityComponentManager->AddComponent<TransformComponent>(mDeathQuoteEntityId, std::make_unique<TransformComponent>(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f/mCoreRenderingService->GetAspectRatio(), 1.0f, 0.0f)));
    mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(EntityNameIdEntry(StringId("game_over_quote"), mDeathQuoteEntityId)));
}

void EffectsManager::CreateRespawnQuoteEntity()
{
    mRespawnQuoteEntityId = mEntityComponentManager->GenerateEntity();
    mEntityComponentManager->AddComponent<ShaderComponent>(mRespawnQuoteEntityId, std::make_unique<ShaderComponent>(StringId("background"), false));
    mEntityComponentManager->AddComponent<AnimationComponent>(mRespawnQuoteEntityId, std::make_unique<AnimationComponent>("misc/respawn_quote", 0.05f, *mResourceManager, false, false));
    mEntityComponentManager->AddComponent<TransformComponent>(mRespawnQuoteEntityId, std::make_unique<TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f / mCoreRenderingService->GetAspectRatio(), 1.0f, 0.0f)));
    mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(EntityNameIdEntry(StringId("respawn_quote"), mRespawnQuoteEntityId)));
}
