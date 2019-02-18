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
#include "../components/FactionComponent.h"
#include "../components/ShaderComponent.h"
#include "../events/NewEntityCreatedEvent.h"
#include "../components/AnimationComponent.h"
#include "../resources/ResourceManager.h"
#include "../resources/TextureResource.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/DamageComponent.h"
#include "../components/RangedShurikenAIComponent.h"


EntityRangedShurikenAttackCommand::EntityRangedShurikenAttackCommand(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mServiceLocator(serviceLocator)
    , mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mResourceManager(serviceLocator.ResolveService<ResourceManager>())
    , mParentEntityId(entityId)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

void EntityRangedShurikenAttackCommand::VExecute()
{
    const auto& entityTransformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mParentEntityId);
    
    const auto shurikenEntityId = mEntityComponentManager.GenerateEntity();
    mEntityComponentManager.AddComponent<ShaderComponent>(shurikenEntityId, std::make_unique<ShaderComponent>(StringId("basic")));
        
    const auto& frameTextureResource = mResourceManager.GetResource<TextureResource>("effects/player_shuriken/idle/0.png");
    AnimationComponent::AnimationsMap mAnimation;
    mAnimation[StringId("idle")].push_back(frameTextureResource.GetGLTextureId());
    
    auto shurikenAnimationComponent = std::make_unique<AnimationComponent>(mAnimation, 100.0f);
    auto shurikenTransformComponent = std::make_unique<TransformComponent>();
    auto shurikenPhysicsComponent = std::make_unique<PhysicsComponent>(PhysicsComponent::BodyType::DYNAMIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(40.0f, 40.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 1000.0f, 0.0f), glm::vec3(-1000.0f, -1000.0f, 0.0f));
    
    if (mEntityComponentManager.GetComponent<AnimationComponent>(mParentEntityId).GetCurrentFacingDirection() == FacingDirection::RIGHT)
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
    
    shurikenTransformComponent->GetScale() = glm::vec3(30.0f, 30.0f, 1.0f);

    const auto& parentEntityFactionGroup = mEntityComponentManager.GetComponent<FactionComponent>(mParentEntityId).GetFactionGroup();

    mEntityComponentManager.AddComponent<PhysicsComponent>(shurikenEntityId, std::move(shurikenPhysicsComponent));
    mEntityComponentManager.AddComponent<FactionComponent>(shurikenEntityId, std::make_unique<FactionComponent>(parentEntityFactionGroup));
    mEntityComponentManager.AddComponent<AnimationComponent>(shurikenEntityId, std::move(shurikenAnimationComponent));
    mEntityComponentManager.AddComponent<TransformComponent>(shurikenEntityId, std::move(shurikenTransformComponent));
    mEntityComponentManager.AddComponent<IAIComponent>(shurikenEntityId, std::make_unique<RangedShurikenAIComponent>(mServiceLocator, shurikenEntityId, 20.0f));
    mEntityComponentManager.AddComponent<DamageComponent>(shurikenEntityId, std::make_unique<DamageComponent>(mParentEntityId, 10.0f, false));
    mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(EntityNameIdEntry(StringId("player_shuriken"), shurikenEntityId)));
}
