//
//  ResourceManager.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "ResourceManager.h"
#include "TextureLoader.h"
#include "FileLoader.h"
#include "BaseResource.h"
#include "../util/TypeTraits.h"
#include "../util/Logging.h"
#include "../util/StringUtils.h"
#include "../util/SDLCommonUtils.h"

#include <map>
#include <string>
#include <cassert>

ResourceManager::ResourceManager()
{
    MapResourceExtensionsToLoaders();
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::LoadResource(const std::string& resourcePath, const bool /* false */)
{
    const auto resourceId = GetTypeHash(resourcePath);
    
    if (mResourceMap.count(resourceId))
    {
        Log(LogType::WARNING, "Resource %s already loaded", resourcePath.c_str());
        return;
    }
    else
    {
        LoadResourceInternal(resourcePath, resourceId);
    }
}

void ResourceManager::LoadResources(const std::vector<const std::string>& resourcePaths, const bool async /* false */)
{
    for (const auto path: resourcePaths)
    {
        LoadResource(path, async);
    }
}

void ResourceManager::UnloadResource(const BaseResource& baseResource)
{
    mResourceMap.erase(baseResource.GetId());
}

void ResourceManager::UnloadResource(const ResourceId resourceId)
{
    mResourceMap.erase(resourceId);
}

BaseResource& ResourceManager::GetResource(const std::string& resourcePath)
{
    const auto resourceHash = GetTypeHash(resourcePath);
    return GetResource(resourceHash);
}

BaseResource& ResourceManager::GetResource(const ResourceId resourceId)
{
    if (mResourceMap.count(resourceId))
    {
        return *mResourceMap[resourceId];
    }
    else
    {
        Log(LogType::ERROR, "Resource not loaded when requested. Aborting");
        assert(false);
    }
}

void ResourceManager::MapResourceExtensionsToLoaders()
{
    auto textureLoader = std::make_shared<TextureLoader>();
    auto fileLoader = std::make_shared<FileLoader>();
    
    mResourceExtensionsToLoadersMap["png"] = textureLoader;
    mResourceExtensionsToLoadersMap["json"] = fileLoader;
}

void ResourceManager::LoadResourceInternal(const std::string& resourcePath, const ResourceId resourceId)
{
    const auto resourceFileExtension = GetFileExtension(resourcePath);
    auto loadedResource = mResourceExtensionsToLoadersMap[resourceFileExtension]->VCreateAndLoadResource(resourcePath, resourceId);
    mResourceMap[resourceId] = std::move(loadedResource);
}
