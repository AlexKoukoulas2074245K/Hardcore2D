//
//  ResourceManager.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "ResourceManager.h"
#include "TextureLoader.h"
#include "TextFileLoader.h"
#include "IResource.h"
#include "../util/TypeTraits.h"
#include "../util/Logging.h"
#include "../util/FileUtils.h"
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

const std::string& ResourceManager::GetRootResourceDirectory() const
{
	return mRootResourceDirectory;
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

void ResourceManager::LoadResources(const std::vector<std::string>& resourceRelativePaths, const bool async /* false */)
{
    for (const auto path: resourceRelativePaths)
    {
        LoadResource(path, async);
    }
}

void ResourceManager::UnloadResource(const std::string& resourceRelativePath)
{
    const auto resourceId = GetTypeHash(resourceRelativePath);
    mResourceMap.erase(resourceId);
}

void ResourceManager::UnloadResource(const ResourceId resourceId)
{
    mResourceMap.erase(resourceId);
}

IResource& ResourceManager::GetResource(const std::string& resourceRelativePath)
{
    const auto resourceId = GetTypeHash(resourceRelativePath);
    return GetResource(resourceId);
}

IResource& ResourceManager::GetResource(const ResourceId resourceId)
{
    if (mResourceMap.count(resourceId))
    {
        return *mResourceMap[resourceId];
    }
    
    Log(LogType::ERROR, "Resource not loaded when requested. Aborting");
    assert(false);
	return *mResourceMap[resourceId];
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
    
    auto loadedResource = mResourceExtensionsToLoadersMap[resourceFileExtension]->VCreateAndLoadResource(mRootResourceDirectory + resourceRelativePath);
    
    mResourceMap[resourceId] = std::move(loadedResource);
}
