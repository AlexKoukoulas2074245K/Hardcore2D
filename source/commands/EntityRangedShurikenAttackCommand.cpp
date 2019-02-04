//
//  EntityRangedShurikenAttackCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#include "EntityRangedShurikenAttackCommand.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/ShaderComponent.h"
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
{
    
}

void EntityRangedShurikenAttackCommand::VExecute()
{
    const auto shurikenEntityId = mEntityComponentManager.GenerateEntity();
    mEntityComponentManager.AddComponent<ShaderComponent>(shurikenEntityId, std::make_unique<ShaderComponent>(StringId("basic")));
    
    AnimationComponent::AnimationsMap mAnimation;
    
    const auto& frameTextureResource = mResourceManager.GetResource<TextureResource>("effects/player_shuriken/idle/0.png");
    mAnimation[StringId("idle")].push_back(frameTextureResource.GetGLTextureId());
    
    auto shurikenAnimationComponent = std::make_unique<AnimationComponent>(mAnimation, 100.0f);
    auto shurikenTransformComponent = std::make_unique<TransformComponent>();
    
    if (shurikenAnimationComponent->GetCurrentFacingDirection() == FacingDirection::RIGHT)
    {
        shurikenTransformComponent->SetParent(mEntityId, glm::vec3(80.0f, 0.0f, 0.0f));
        shurikenAnimationComponent->SetFacingDirection(FacingDirection::RIGHT);
    }
    else
    {
        shurikenTransformComponent->SetParent(mEntityId, glm::vec3(-80.0f, 0.0f, 0.0f));
        shurikenAnimationComponent->SetFacingDirection(FacingDirection::LEFT);
    }
    
    shurikenTransformComponent->GetScale() = glm::vec3(80.0f, 200.0f, 1.0f);
    
    mEntityComponentManager.AddComponent<PhysicsComponent>(shurikenEntityId, std::make_unique<PhysicsComponent>
                                                           (PhysicsComponent::BodyType::DYNAMIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(48.0f, 48.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 1000.0f, 0.0f), glm::vec3(-1000.0f, -1000.0f, 0.0f)));
    
    mEntityComponentManager.AddComponent<AnimationComponent>(swingEntityId, std::move(swingAnimationComponent));
    mEntityComponentManager.AddComponent<TransformComponent>(swingEntityId, std::move(swingTransformComponent));
    mEntityComponentManager.AddComponent<IAIComponent>(swingEntityId, std::make_unique<MeleeSwingAIComponent>(mServiceLocator, swingEntityId, 0.15f));
    mEntityComponentManager.AddComponent<DamageComponent>(swingEntityId, std::make_unique<DamageComponent>(mEntityId, 1.0f));
    mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(EntityNameIdEntry(StringId("player_swing"), swingEntityId)));
}
