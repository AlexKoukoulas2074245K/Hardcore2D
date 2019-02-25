//
//  LevelFactory.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 27/01/2019.
//

#include "LevelFactory.h"
#include "Level.h"
#include "../components/EntityComponentManager.h"
#include "../components/BasicNinjaEnemyAIComponent.h"
#include "../components/HorizontalMovingPlatformAIComponent.h"
#include "../components/VerticalMovingPlatformAIComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/DamageComponent.h"
#include "../components/FactionComponent.h"
#include "../components/HealthComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/ShaderComponent.h"
#include "../components/TransformComponent.h"
#include "../ServiceLocator.h"
#include "../resources/TextFileResource.h"
#include "../resources/ResourceManager.h"
#include "../util/Logging.h"
#include "../util/StringUtils.h"
#include "../util/FileUtils.h"
#include "../game/GameTypeTraits.h"

#include <glm/vec3.hpp>
#include <json.hpp>
#include <iostream>
#include <unordered_map>
#include <SDL.h>


static const float CHARACTER_SCALE_MULTIPLIER = 2.5f;
static const std::string LEVEL_DIRECTORY = "levels/";
static const std::unordered_map<std::string, PhysicsComponent::BodyType> sStringToPhysicsBodyType = 
{
    { "DYNAMIC", PhysicsComponent::BodyType::DYNAMIC },
    { "KINEMATIC", PhysicsComponent::BodyType::KINEMATIC },
    { "STATIC", PhysicsComponent::BodyType::STATIC }
};

LevelFactory::LevelFactory(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
    
}

std::unique_ptr<Level> LevelFactory::CreateLevel(const std::string& levelPath)
{
    const auto startTime = static_cast<float>(SDL_GetTicks());
    
    auto& resourceManager = mServiceLocator.ResolveService<ResourceManager>();
    auto& entityComponentManager = mServiceLocator.ResolveService<EntityComponentManager>();
    
    const auto& levelFileResource = resourceManager.GetResource<TextFileResource>(resourceManager.LoadResource(LEVEL_DIRECTORY + levelPath));
    const auto levelJson = nlohmann::json::parse(levelFileResource.GetContents());
    
    const auto levelHorizontalBounds = glm::vec2(levelJson["horBounds"]["left"].get<float>(),
                                                 levelJson["horBounds"]["right"].get<float>());
    
    const auto levelVerticalBounds = glm::vec2(levelJson["verBounds"]["bottom"].get<float>(),
                                               levelJson["verBounds"]["top"].get<float>());
    
    for (const auto& cachedAnimationRootDirectory : levelJson["cached_animation_directories"])
    {        
        const auto animationRootDirectories = GetAllFilenamesInDirectory(ResourceManager::RES_ROOT + cachedAnimationRootDirectory.get<std::string>());
        for (const auto& animationDirectory : animationRootDirectories)
        {
            const auto animationFrameFiles = GetAllFilenamesInDirectory(ResourceManager::RES_ROOT + cachedAnimationRootDirectory.get<std::string>() + "/" + animationDirectory + "/idle/");
            for (const auto& animationFrameFile: animationFrameFiles)
            {
                resourceManager.LoadResource(cachedAnimationRootDirectory.get<std::string>() + animationDirectory + "/idle/" + animationFrameFile);
            }
        }        
    }

    std::vector<EntityNameIdEntry> levelEntityEntries;    
    for (auto& entity: levelJson["entities"])
    {
        const auto entityName = entity["name"].get<std::string>();
        const auto entityId = entityComponentManager.GenerateEntity();
        
        const auto entityFaction = (StringStartsWith(entityName, "player") || StringStartsWith(entityName, "ally-")) ? FactionGroup::ALLIES : FactionGroup::ENEMIES;
        entityComponentManager.AddComponent<FactionComponent>(entityId, std::make_unique<FactionComponent>(entityFaction));

        for (auto& componentEntry: entity["components"].items())
        {
            const auto& componentName = componentEntry.key();
            const auto& componentProperties = componentEntry.value();
            
            if (StringEndsWith(componentName, "AIComponent"))
            {
                if (componentName == "BasicNinjaEnemyAIComponent")
                {
                    entityComponentManager.AddComponent<IAIComponent>(entityId, std::make_unique<BasicNinjaEnemyAIComponent>(mServiceLocator, entityId));
                }
                else if (componentName == "VerticalMovingPlatformAIComponent")
                {
                    entityComponentManager.AddComponent<IAIComponent>(entityId, std::make_unique<VerticalMovingPlatformAIComponent>(mServiceLocator, entityId));
                }
                else if (componentName == "HorizontalMovingPlatformAIComponent")
                {
                    entityComponentManager.AddComponent<IAIComponent>(entityId, std::make_unique<HorizontalMovingPlatformAIComponent>(mServiceLocator, entityId));
                }
            }
            else if (componentName == "AnimationComponent")
            {
                const auto animationRootDirectory = componentProperties["path"].get<std::string>();
                const auto animationFrameDuration = componentProperties["animationFrameDuration"].get<float>();
                entityComponentManager.AddComponent<AnimationComponent>(entityId, std::make_unique<AnimationComponent>(animationRootDirectory, animationFrameDuration, resourceManager));
            }
            else if (componentName == "DamageComponent")
            {
                const auto damage = componentProperties["damage"].get<float>();
                const auto canDamageSameEntityMultipleTimes = componentProperties["canDamageSameEntityMultipleTimes"].get<bool>();
                entityComponentManager.AddComponent<DamageComponent>(entityId, std::make_unique<DamageComponent>(entityId, damage, canDamageSameEntityMultipleTimes));
            }
            else if (componentName == "HealthComponent")
            {
                const auto health = componentProperties["health"].get<float>();
                const auto invincibilityDuration = componentProperties["invincibilityDuration"].get<float>();
                entityComponentManager.AddComponent<HealthComponent>(entityId, std::make_unique<HealthComponent>(health, invincibilityDuration));
            }
            else if (componentName == "PhysicsComponent")
            {
                const auto bodyType = sStringToPhysicsBodyType.at(componentProperties["bodyType"].get<std::string>());
                
                const auto hitBoxCenter = glm::vec2(componentProperties["hitBox"]["centerPoint"][0].get<float>(), componentProperties["hitBox"]["centerPoint"][1].get<float>());                
                const auto hitBoxDimensions = glm::vec2(componentProperties["hitBox"]["dimensions"][0].get<float>(), componentProperties["hitBox"]["dimensions"][1].get<float>());
                const PhysicsComponent::Hitbox hitBox(hitBoxCenter, hitBoxDimensions);

                const auto gravity = glm::vec3(componentProperties["gravity"][0].get<float>(),
                                               componentProperties["gravity"][1].get<float>(),
                                               componentProperties["gravity"][2].get<float>());

                const auto maxVelocity = glm::vec3(componentProperties["maxVelocity"][0].get<float>(),
                                                   componentProperties["maxVelocity"][1].get<float>(),
                                                   componentProperties["maxVelocity"][2].get<float>());

                const auto minVelocity = glm::vec3(componentProperties["minVelocity"][0].get<float>(),
                                                   componentProperties["minVelocity"][1].get<float>(),
                                                   componentProperties["minVelocity"][2].get<float>());

                entityComponentManager.AddComponent<PhysicsComponent>(entityId, std::make_unique<PhysicsComponent>(bodyType, hitBox, gravity, maxVelocity, minVelocity));
            }
            else if (componentName == "ShaderComponent")
            {
                const auto shaderName = componentProperties["shaderName"].get<std::string>();
                entityComponentManager.AddComponent<ShaderComponent>(entityId, std::make_unique<ShaderComponent>(StringId(shaderName)));
            }
            else if (componentName == "TransformComponent")
            {
                const auto translation = glm::vec3(componentProperties["translation"][0].get<float>(),
                                                   componentProperties["translation"][1].get<float>(),
                                                   componentProperties["translation"][2].get<float>());

                const auto rotation = glm::vec3(componentProperties["rotation"][0].get<float>(),
                                                componentProperties["rotation"][1].get<float>(),
                                                componentProperties["rotation"][2].get<float>());
                                
                const auto scale  = glm::vec3(componentProperties["scale"][0].get<float>(),
                                              componentProperties["scale"][1].get<float>(),
                                              componentProperties["scale"][2].get<float>());

                entityComponentManager.AddComponent<TransformComponent>(entityId, std::make_unique<TransformComponent>(translation, rotation, scale));
            } 
        }
        
        // Apply scale (and hitbox) multiplier to character entities due to level loading
        if (entityComponentManager.HasComponent<AnimationComponent>(entityId) && StringStartsWith(entityComponentManager.GetComponent<AnimationComponent>(entityId).GetRootAnimationsPath(), "characters"))
        {
            if (entityComponentManager.HasComponent<PhysicsComponent>(entityId))
            {
                auto& physicsComponent = entityComponentManager.GetComponent<PhysicsComponent>(entityId);
                physicsComponent.GetHitBox() = PhysicsComponent::Hitbox(physicsComponent.GetHitBox().mCenterPoint, physicsComponent.GetHitBox().mDimensions * CHARACTER_SCALE_MULTIPLIER);
            }

            if (entityComponentManager.HasComponent<TransformComponent>(entityId))
            {
                auto& transformComponent = entityComponentManager.GetComponent<TransformComponent>(entityId);
                transformComponent.GetScale().x *= CHARACTER_SCALE_MULTIPLIER;
                transformComponent.GetScale().y *= CHARACTER_SCALE_MULTIPLIER;
            }
        }

        levelEntityEntries.emplace_back(EntityNameIdEntry(StringId(entityName), entityId));
    }
    
    Log(LogType::INFO, "Created level %s in %.6f ms", levelPath.c_str(), (static_cast<float>(SDL_GetTicks()) - startTime));
    return std::unique_ptr<Level>(new Level(mServiceLocator, levelEntityEntries, levelHorizontalBounds, levelVerticalBounds));
}
