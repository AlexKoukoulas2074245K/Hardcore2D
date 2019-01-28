//
//  LevelFactory.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 27/01/2019.
//

#include "LevelFactory.h"
#include "Level.h"
#include "../components/EntityComponentManager.h"
#include "../ServiceLocator.h"
#include "../resources/TextFileResource.h"
#include "../resources/ResourceManager.h"
#include "../util/Logging.h"

#include <json.hpp>
#include <iostream>
static const std::string LEVEL_DIRECTORY = "levels/";

LevelFactory::LevelFactory(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
{
    
}

std::unique_ptr<Level> LevelFactory::CreateLevel(const std::string& levelPath)
{
    auto& resourceManager = mServiceLocator.ResolveService<ResourceManager>();
    auto& entityComponentManager = mServiceLocator.ResolveService<EntityComponentManager>();
    
    const auto& levelFileResource = resourceManager.GetResource<TextFileResource>(resourceManager.LoadResource(LEVEL_DIRECTORY + levelPath));
    const auto levelJson = nlohmann::json::parse(levelFileResource.GetContents());
    
    std::vector<Level::EntityNameIdEntry> levelEntityEntries;
    
    for (auto& entity: levelJson["entities"])
    {
        const auto entityName = entity["name"].get<std::string>();
        const auto entityId = entityComponentManager.GenerateEntity();
        
        for (auto& componentEntry: entity["components"].items())
        {
            std::cout << componentEntry.key() << ", " << componentEntry.value() << std::endl;
        }
        
        levelEntityEntries.emplace_back(Level::EntityNameIdEntry(StringId(entityName), entityId));
    }
    
    return std::unique_ptr<Level>(new Level(levelEntityEntries));
}
