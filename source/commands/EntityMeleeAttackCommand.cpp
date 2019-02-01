//
//  EntityMeleeAttackCommand.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#include "EntityMeleeAttackCommand.h"
#include "../ServiceLocator.h"
#include "../resources/ResourceManager.h"
#include "../resources/TextureResource.h"
#include "../components/EntityComponentManager.h"
#include "../components/PhysicsComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/ShaderComponent.h"
#include "../util/MathUtils.h"

const StringId EntityMeleeAttackCommand::COMMAND_CLASS_ID("EntityMeleeAttackCommand");

EntityMeleeAttackCommand::EntityMeleeAttackCommand(const ServiceLocator& serviceLocator, const EntityId entityId)
    : mEntityComponentManager(serviceLocator.ResolveService<EntityComponentManager>())
    , mResourceManager(serviceLocator.ResolveService<ResourceManager>())
    , mEntityId(entityId)
{
    
}

void EntityMeleeAttackCommand::Execute()
{    
    auto& entityAnimationComponent = mEntityComponentManager.GetComponent<AnimationComponent>(mEntityId);
    entityAnimationComponent.PlayAnimationOnce(StringId("melee"));

    const auto playerSwingEntityId = mEntityComponentManager.GenerateEntity();    
    mEntityComponentManager.AddComponent<ShaderComponent>(playerSwingEntityId, std::make_unique<ShaderComponent>(StringId("basic")));
 
    AnimationComponent::AnimationsMap mMeleeAnimations;
    for (auto i = 0; i < 7; ++i)
    {
        const auto& frameTextureResource = mResourceManager.GetResource<TextureResource>(ResourceManager::RES_ROOT + "effects/player_swing/idle/" + std::to_string(i) + ".png");
        mMeleeAnimations[StringId("idle")].push_back(frameTextureResource.GetGLTextureId());
    }

    mEntityComponentManager.AddComponent<AnimationComponent>(playerSwingEntityId, std::make_unique<AnimationComponent>(mMeleeAnimations, 0.04f));
    mEntityComponentManager.AddComponent<PhysicsComponent>(playerSwingEntityId, std::make_unique<PhysicsComponent>
        (PhysicsComponent::BodyType::DYNAMIC, PhysicsComponent::Hitbox(, glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& maxVelocity, const glm::vec3& minVelocity));
}

StringId EntityMeleeAttackCommand::GetCommandClassId() const
{
    return COMMAND_CLASS_ID;
}
