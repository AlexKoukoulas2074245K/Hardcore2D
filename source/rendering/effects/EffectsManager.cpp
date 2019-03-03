//
//  EffectsManager.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/02/2019.
//

#include "EffectsManager.h"
#include "../../ServiceLocator.h"
#include "../../components/EntityComponentManager.h"
#include "../../events/EventCommunicator.h"
#include "../../events/NewEntityCreatedEvent.h"
#include "../../components/ShaderComponent.h"
#include "../../components/AnimationComponent.h"
#include "../../components/PhysicsComponent.h"
#include "../../components/TransformComponent.h"
#include "../../components/BloodDropAIComponent.h"
#include "../../components/FactionComponent.h"
#include "../../resources/ResourceManager.h"
#include "../../resources/TextureResource.h"
#include "../../util/MathUtils.h"

EffectsManager::EffectsManager(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
}

EffectsManager::~EffectsManager()
{
}

bool EffectsManager::VInitialize()
{
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
    mResourceManager = &(mServiceLocator.ResolveService<ResourceManager>());
    mEventCommunicator = mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    
    return true;
}

void EffectsManager::PlayEffect(const glm::vec3& effectOrigin, const EffectType effectType)
{
    switch (effectType)
    {
        case EffectType::BLOOD_SPURT: CreateBloodSpurtEffect(effectOrigin); break;
    }
}

void EffectsManager::CreateBloodSpurtEffect(const glm::vec3& effectOrigin)
{
    const auto bloodDropsCount = RandomInt(4, 8);
    
    for (int i = 0; i < bloodDropsCount; ++i)
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
        
        bloodDropPhysicsComponent->GetVelocity() = glm::vec3(randomVelocityX, randomVelocityY, 0.0f);
        mEntityComponentManager->AddComponent<PhysicsComponent>(bloodDropEntityId, std::move(bloodDropPhysicsComponent));
        
        auto bloodDropTransformComponent = std::make_unique<TransformComponent>();
        bloodDropTransformComponent->GetTranslation().x = effectOrigin.x;
        bloodDropTransformComponent->GetTranslation().y = effectOrigin.y;
        bloodDropTransformComponent->GetScale() = glm::vec3(8.0f, 8.0f, 1.0f);
        
        mEntityComponentManager->AddComponent<FactionComponent>(bloodDropEntityId, std::make_unique<FactionComponent>(FactionGroup::ALLIES));
        mEntityComponentManager->AddComponent<TransformComponent>(bloodDropEntityId, std::move(bloodDropTransformComponent));
        mEntityComponentManager->AddComponent<IAIComponent>(bloodDropEntityId, std::make_unique<BloodDropAIComponent>(mServiceLocator, bloodDropEntityId, 0.5f));
        mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(EntityNameIdEntry(StringId("blood_drop"), bloodDropEntityId)));
    }
}
