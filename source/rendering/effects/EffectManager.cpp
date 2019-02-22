//
//  EffectManager.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/02/2019.
//

#include "EffectManager.h"
#include "../../ServiceLocator.h"
#include "../../components/EntityComponentManager.h"
#include "../../events/EventCommunicator.h"
#include "../../events/NewEntityCreatedEvent.h"
#include "../../components/ShaderComponent.h"
#include "../../components/AnimationComponent.h"
#include "../../components/PhysicsComponent.h"
#include "../../components/TransformComponent.h"
#include "../../components/BloodDropAIComponent.h"
#include "../../resources/ResourceManager.h"
#include "../../resources/TextureResource.h"

EffectManager::EffectManager(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
}

EffectManager::~EffectManager()
{
}

bool EffectManager::VInitialize()
{
    mEntityComponentManager = &(mServiceLocator.ResolveService<EntityComponentManager>());
    mResourceManager = &(mServiceLocator.ResolveService<ResourceManager>());
    mEventCommunicator = mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator();
    
    return true;
}

void EffectManager::PlayEffect(const EntityId originEntityId, const EffectType effectType)
{
    switch (effectType)
    {
        case EffectType::BLOOD_SPURT: CreateBloodSpurtEffect(originEntityId); break;
    }
}

void EffectManager::CreateBloodSpurtEffect(const EntityId originEntityId)
{
    const auto bloodDropEntityId = mEntityComponentManager->GenerateEntity();
    mEntityComponentManager->AddComponent<ShaderComponent>(bloodDropEntityId, std::make_unique<ShaderComponent>(StringId("basic")));

    const auto& frameTextureResource = mResourceManager->GetResource<TextureResource>("effects/blood_drop/idle/0.png");
    AnimationComponent::AnimationsMap mAnimation;
    mAnimation[StringId("idle")].push_back(frameTextureResource.GetGLTextureId());
    mEntityComponentManager->AddComponent<AnimationComponent>(bloodDropEntityId, std::make_unique<AnimationComponent>(mAnimation, 100.0f));
    
    auto bloodDropPhysicsComponent = std::make_unique<PhysicsComponent>
        (PhysicsComponent::BodyType::DYNAMIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)), glm::vec3(0.0f, -1680.0f, 0.0f), glm::vec3(500.0f, 500.0f, 0.0f), glm::vec3(-500.0f, -500.0f, 0.0f));
    bloodDropPhysicsComponent->GetVelocity() = glm::vec3(-200.0f, 200.0f, 0.0f);
    mEntityComponentManager->AddComponent<PhysicsComponent>(bloodDropEntityId, std::move(bloodDropPhysicsComponent));

    auto bloodDropTransformComponent = std::make_unique<TransformComponent>();
    bloodDropTransformComponent->SetParent(originEntityId, glm::vec3(0.0f, 0.0f, 0.0f));
    bloodDropTransformComponent->GetScale() = glm::vec3(32.0f, 32.0f, 1.0f);
        
    mEntityComponentManager->AddComponent<TransformComponent>(bloodDropEntityId, std::move(bloodDropTransformComponent));
    mEntityComponentManager->AddComponent<IAIComponent>(bloodDropEntityId, std::make_unique<BloodDropAIComponent>(mServiceLocator, bloodDropEntityId, 5.0f));
    mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(EntityNameIdEntry(StringId("blood_drop"), bloodDropEntityId)));
}
