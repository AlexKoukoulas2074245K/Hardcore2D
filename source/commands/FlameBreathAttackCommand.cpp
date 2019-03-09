//
//  FlameBreathAttackCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 09/03/2019.
//

#include "FlameBreathAttackCommand.h"
#include "../ServiceLocator.h"
#include "../components/EntityComponentManager.h"
#include "../components/AnimationComponent.h"
#include "../components/ShaderComponent.h"
#include "../components/TransformComponent.h"
#include "../components/DamageComponent.h"
#include "../components/FlameBreathAIComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/FactionComponent.h"
#include "../events/EventCommunicator.h"
#include "../events/NewEntityCreatedEvent.h"
#include "../resources/ResourceManager.h"

FlameBreathAttackCommand::FlameBreathAttackCommand(const ServiceLocator& serviceLocator, const EntityId parentEntityId)
    : mServiceLocator(serviceLocator)
    , mEntityComponentManager(mServiceLocator.ResolveService<EntityComponentManager>())
    , mParentEntityId(parentEntityId)
    , mEventCommunicator(mServiceLocator.ResolveService<EventCommunicationService>().CreateEventCommunicator())
{
    
}

void FlameBreathAttackCommand::VExecute()
{
    const auto& parentAnimationComponent = mEntityComponentManager.GetComponent<AnimationComponent>(mParentEntityId);
    mEntityComponentManager.GetComponent<AnimationComponent>(mParentEntityId).PlayAnimation(StringId("flame_breath"), false, true, AnimationComponent::AnimationPriority::HIGH);
    
    const auto flameBreathEntityId = mEntityComponentManager.GenerateEntity();
    mEntityComponentManager.AddComponent<PhysicsComponent>(flameBreathEntityId, std::make_unique<PhysicsComponent>
                                                           (PhysicsComponent::BodyType::DYNAMIC, PhysicsComponent::Hitbox(glm::vec2(0.0f, 0.0f), glm::vec2(230.0f, 100.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 1000.0f, 0.0f), glm::vec3(-1000.0f, -1000.0f, 0.0f)));
    
    auto flameBreathTrasformComponent = std::make_unique<TransformComponent>();
    flameBreathTrasformComponent->SetParent(mParentEntityId, glm::vec3(parentAnimationComponent.GetCurrentFacingDirection() == FacingDirection::RIGHT ? 140.0f: -140.0f, 20.0f, 0.0f));
    flameBreathTrasformComponent->GetScale() = glm::vec3(80.0f, 160.0f, 1.0f);
    
    const auto& parentEntityFactionGroup = mEntityComponentManager.GetComponent<FactionComponent>(mParentEntityId).GetFactionGroup();
    
    mEntityComponentManager.AddComponent<FactionComponent>(flameBreathEntityId, std::make_unique<FactionComponent>(parentEntityFactionGroup));
    mEntityComponentManager.AddComponent<TransformComponent>(flameBreathEntityId, std::move(flameBreathTrasformComponent));
    mEntityComponentManager.AddComponent<IAIComponent>(flameBreathEntityId, std::make_unique<FlameBreathAIComponent>(mServiceLocator, mParentEntityId, flameBreathEntityId, 0.5f));
    mEntityComponentManager.AddComponent<DamageComponent>(flameBreathEntityId, std::make_unique<DamageComponent>(mParentEntityId, 50.0f, false));
    mEventCommunicator->DispatchEvent(std::make_unique<NewEntityCreatedEvent>(EntityNameIdEntry(StringId("flame_breath"), flameBreathEntityId)));
}
