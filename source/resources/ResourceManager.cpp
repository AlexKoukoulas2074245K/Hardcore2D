//
//  ResourceManager.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "ResourceManager.h"
#include "TextureLoader.h"
#include "TextFileLoader.h"
#include "IResource.h"
#include "../util/TypeTraits.h"
#include "../util/Logging.h"
#include "../util/StringUtils.h"
#include "../util/SDLMessageBoxUtils.h"

#include <map>
#include <string>
#include <cassert>

ResourceManager::ResourceManager(const std::string& rootResourceDirectory)
    : mRootResourceDirectory(rootResourceDirectory)
    , mTextFileLoader(new TextFileLoader)
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
    if (!mTextFileLoader->Initialize()) return false;
    return true;
}

ResourceId ResourceManager::LoadResource(const std::string& resourceRelativePath, const bool /* false */)
{
    const auto resourceId = GetTypeHash(resourceRelativePath);
    
    if (mResourceMap.count(resourceId))
    {
        Log(LogType::WARNING, "Resource %s already loaded", resourceRelativePath.c_str());
        return 0;
    }
    else
    {
        LoadResourceInternal(resourceRelativePath, resourceId);
        return resourceId;
    }
}

void ResourceManager::LoadResources(const std::vector<const std::string>& resourceRelativePaths, const bool async /* false */)
{
    for (const auto path: resourceRelativePaths)
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

IResource& ResourceManager::GetResource(const std::string& resourceRelativePath)
{
    const auto resourceHash = GetTypeHash(resourceRelativePath);
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
    mResourceExtensionsToLoadersMap["json"] = mTextFileLoader.get();
    mResourceExtensionsToLoadersMap["vs"] = mTextFileLoader.get();
    mResourceExtensionsToLoadersMap["fs"] = mTextFileLoader.get();
}

void ResourceManager::LoadResourceInternal(const std::string& resourceRelativePath, const ResourceId resourceId)
{
    const auto resourceFileExtension = GetFileExtension(resourceRelativePath);
    
    auto loadedResource = mResourceExtensionsToLoadersMap[resourceFileExtension]->VCreateAndLoadResource(mRootResourceDirectory + resourceRelativePath, resourceId);
    
    mResourceMap[resourceId] = std::move(loadedResource);
}
