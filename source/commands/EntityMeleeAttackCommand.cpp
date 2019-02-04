//
//  EntityMeleeAttackCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#include "EntityMeleeAttackCommand.h"
#include "../ServiceLocator.h"
#include "../events/EventCommunicator.h"
#include "../events/NewEntityCreatedEvent.h"
#include "../resources/ResourceManager.h"
#include "../resources/TextureResource.h"
#include "../commands/SetEntityFacingDirectionCommand.h"
#include "../components/MeleeSwingAIComponent.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/TransformComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/ShaderComponent.h"
#include "../components/DamageComponent.h"
#include "../util/MathUtils.h"

EntityMeleeAttackCommand::EntityMeleeAttackCommand(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mServiceLocator(serviceLocator)
    , mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mResourceManager(serviceLocator.ResolveService<ResourceManager>())
    , mEntityId(entityId)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

void EntityMeleeAttackCommand::VExecute()
{
    auto& entityAnimationComponent = mEntityComponentManager.GetComponent<AnimationComponent>(mEntityId);
    entityAnimationComponent.PlayAnimationOnce(StringId("melee"));

    const auto swingEntityId = mEntityComponentManager.GenerateEntity();
    mEntityComponentManager.AddComponent<ShaderComponent>(swingEntityId, std::make_unique<ShaderComponent>(StringId("basic")));
 
    AnimationComponent::AnimationsMap mMeleeAnimations;
    for (auto i = 0; i < 7; ++i)
    {
        const auto& frameTextureResource = mResourceManager.GetResource<TextureResource>("effects/player_swing/idle/" + std::to_string(i) + ".png");
        mMeleeAnimations[StringId("idle")].push_back(frameTextureResource.GetGLTextureId());
    }
    
    auto swingAnimationComponent = std::make_unique<AnimationComponent>(mMeleeAnimations, 0.0214f);
    mEntityComponentManager.AddComponent<PhysicsComponent>(swingEntityId, std::make_unique<PhysicsComponent>
                                                           (PhysicsComponent::BodyType::DYNAMIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(80.0f, 200.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 1000.0f, 0.0f), glm::vec3(-1000.0f, -1000.0f, 0.0f)));
    
    auto swingTransformComponent = std::make_unique<TransformComponent>();
    
    if (entityAnimationComponent.GetCurrentFacingDirection() == FacingDirection::RIGHT)
    {
        swingTransformComponent->SetParent(mEntityId, glm::vec3(80.0f, 0.0f, 0.0f));
        swingAnimationComponent->SetFacingDirection(FacingDirection::RIGHT);
    }
    else
    {
        swingTransformComponent->SetParent(mEntityId, glm::vec3(-80.0f, 0.0f, 0.0f));
        swingAnimationComponent->SetFacingDirection(FacingDirection::LEFT);
    }
    
    swingTransformComponent->GetScale() = glm::vec3(80.0f, 200.0f, 1.0f);    
    
    mEntityComponentManager.AddComponent<AnimationComponent>(swingEntityId, std::move(swingAnimationComponent));
    mEntityComponentManager.AddComponent<TransformComponent>(swingEntityId, std::move(swingTransformComponent));
    mEntityComponentManager.AddComponent<IAIComponent>(swingEntityId, std::make_unique<MeleeSwingAIComponent>(mServiceLocator, swingEntityId, 0.15f));
    mEntityComponentManager.AddComponent<DamageComponent>(swingEntityId, std::make_unique<DamageComponent>(mEntityId, 1.0f));
    mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(EntityNameIdEntry(StringId("player_swing"), swingEntityId)));
}
