//
//  EntityRangedShurikenAttackCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#include "EntityRangedShurikenAttackCommand.h"
#include "../ServiceLocator.h"
#include "../events/NewEntityCreatedEvent.h"
#include "../events/EventCommunicator.h"
#include "../components/EntityComponentManager.h"
#include "../components/ShaderComponent.h"
#include "../events/NewEntityCreatedEvent.h"
#include "../components/AnimationComponent.h"
#include "../resources/ResourceManager.h"
#include "../resources/TextureResource.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/DamageComponent.h"

EntityRangedShurikenAttackCommand::EntityRangedShurikenAttackCommand(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mResourceManager(serviceLocator.ResolveService<ResourceManager>())
    , mEntityId(entityId)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

void EntityRangedShurikenAttackCommand::VExecute()
{
    const auto& entityTransformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mEntityId);
    
    const auto shurikenEntityId = mEntityComponentManager.GenerateEntity();
    mEntityComponentManager.AddComponent<ShaderComponent>(shurikenEntityId, std::make_unique<ShaderComponent>(StringId("basic")));
        
    const auto& frameTextureResource = mResourceManager.GetResource<TextureResource>("effects/player_shuriken/idle/0.png");
    AnimationComponent::AnimationsMap mAnimation;
    mAnimation[StringId("idle")].push_back(frameTextureResource.GetGLTextureId());
    
    auto shurikenAnimationComponent = std::make_unique<AnimationComponent>(mAnimation, 100.0f);
    auto shurikenTransformComponent = std::make_unique<TransformComponent>();
    auto shurikenPhysicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::DYNAMIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(40.0f, 40.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 1000.0f, 0.0f), glm::vec3(-1000.0f, -1000.0f, 0.0f));
    
    if (mEntityComponentManager.GetComponent<AnimationComponent>(mEntityId).GetCurrentFacingDirection() == FacingDirection::RIGHT)
    {
        shurikenTransformComponent->GetTranslation() = entityTransformComponent.GetTranslation();
        shurikenTransformComponent->GetTranslation().x += 15.0f;
        shurikenAnimationComponent->SetFacingDirection(FacingDirection::RIGHT);
        shurikenPhysicsComponent->GetAngularVelocity() = 10.0f;
        shurikenPhysicsComponent->GetVelocity().x = 1000.0f;
    }
    else
    {
        shurikenTransformComponent->GetTranslation() = entityTransformComponent.GetTranslation();
        shurikenTransformComponent->GetTranslation().x += -15.0f;
        shurikenAnimationComponent->SetFacingDirection(FacingDirection::LEFT);
        shurikenPhysicsComponent->GetAngularVelocity() = -10.0f;
        shurikenPhysicsComponent->GetVelocity().x = -1000.0f;
    }
    
    shurikenTransformComponent->GetScale() = glm::vec3(40.0f, 40.0f, 1.0f);

    mEntityComponentManager.AddComponent<PhysicsComponent>(shurikenEntityId, std::move(shurikenPhysicsComponent));
    
    mEntityComponentManager.AddComponent<AnimationComponent>(shurikenEntityId, std::move(shurikenAnimationComponent));
    mEntityComponentManager.AddComponent<TransformComponent>(shurikenEntityId, std::move(shurikenTransformComponent));
    //mEntityComponentManager.AddComponent<IAIComponent>(swingEntityId, std::make_unique<MeleeSwingAIComponent>(mServiceLocator, shurikenEntityId, 0.15f));
    mEntityComponentManager.AddComponent<DamageComponent>(shurikenEntityId, std::make_unique<DamageComponent>(mEntityId, 10.0f));
    mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(EntityNameIdEntry(StringId("player_shuriken"), shurikenEntityId)));
}