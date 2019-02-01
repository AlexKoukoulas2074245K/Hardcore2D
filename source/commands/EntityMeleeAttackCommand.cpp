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
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/TransformComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/ShaderComponent.h"
#include "../util/MathUtils.h"

const StringId EntityMeleeAttackCommand::COMMAND_CLASS_ID("EntityMeleeAttackCommand");

EntityMeleeAttackCommand::EntityMeleeAttackCommand(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mResourceManager(serviceLocator.ResolveService<ResourceManager>())
    , mEntityId(entityId)
    , mEventCommunicator(serviceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

void EntityMeleeAttackCommand::Execute()
{
    const auto& entityTransformComponent = mEntityComponentManager.GetComponent<TransformComponent>(mEntityId);
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

    mEntityComponentManager.AddComponent<AnimationComponent>(swingEntityId, std::make_unique<AnimationComponent>(mMeleeAnimations, 0.04f));
    mEntityComponentManager.AddComponent<PhysicsComponent>(swingEntityId, std::make_unique<PhysicsComponent>
                                                           (PhysicsComponent::BodyType::DYNAMIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(20.0f, 160.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 1000.0f, 0.0f), glm::vec3(-1000.0f, 1000.0f, 0.0f)));
    
    auto swingTransformComponent = std::make_unique<TransformComponent>();
    swingTransformComponent->SetParent(&entityTransformComponent);
    swingTransformComponent->GetScale() = glm::vec3(20.0f, 160.0f, 1.0f);
    swingTransformComponent->GetTranslation() = glm::vec3(100.0f, 0.0f, 0.0f);
    mEntityComponentManager.AddComponent<TransformComponent>(swingEntityId, std::move(swingTransformComponent));
    
    mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(swingEntityId));
}

StringId EntityMeleeAttackCommand::GetCommandClassId() const
{
    return COMMAND_CLASS_ID;
}
