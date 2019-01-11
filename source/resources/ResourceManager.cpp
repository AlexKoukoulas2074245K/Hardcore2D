//
//  ResourceManager.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "ResourceManager.h"
#include "TextureLoader.h"
#include "FileLoader.h"
#include "IResource.h"
#include "../util/TypeTraits.h"
#include "../util/Logging.h"
#include "../util/StringUtils.h"
#include "../util/SDLCommonUtils.h"

#include <map>
#include <string>
#include <cassert>

ResourceManager::ResourceManager(const std::string& rootResourceDirectory)
    : mRootResourceDirectory(rootResourceDirectory)
    , mFileLoader(new FileLoader)
    , mTextureLoader(new TextureLoader)

{
    MapResourceExtensionsToLoaders();
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::InitializeResourceLoaders()
{
    if (!mTextureLoader->Initialize()) return false;
    if (!mFileLoader->Initialize()) return false;
    return true;
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

void ResourceManager::UnloadResource(const IResource& IResource)
{
    mResourceMap.erase(IResource.GetId());
}

void ResourceManager::UnloadResource(const ResourceId resourceId)
{
    mResourceMap.erase(resourceId);
}

IResource& ResourceManager::GetResource(const std::string& resourcePath)
{
    const auto resourceHash = GetTypeHash(resourcePath);
    return GetResource(resourceHash);
}

IResource& ResourceManager::GetResource(const ResourceId resourceId)
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
    mResourceExtensionsToLoadersMap["png"] = mTextureLoader.get();
    mResourceExtensionsToLoadersMap["json"] = mFileLoader.get();
}

void ResourceManager::LoadResourceInternal(const std::string& resourcePath, const ResourceId resourceId)
{
    const auto resourceFileExtension = GetFileExtension(resourcePath);
    auto loadedResource = mResourceExtensionsToLoadersMap[resourceFileExtension]->VCreateAndLoadResource(resourcePath, resourceId);
    mResourceMap[resourceId] = std::move(loadedResource);
}
