//
//  ResourceManager.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef ResourceManager_h
#define ResourceManager_h

#include "../IService.h"
#include "../util/TypeTraits.h"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <map>

class IResource;
class IResourceLoader;
class ResourceManager final: public IService
{
    friend class App;
public:
    ~ResourceManager();
    
    bool InitializeResourceLoaders();
    
    ResourceId LoadResource(const std::string& resourceRelativePath, const bool async = false);
    void LoadResources(const std::vector<const std::string>& resourceRelativePaths, const bool async = false);
    
    void UnloadResource(const IResource& IResource);
    void UnloadResource(const ResourceId resourceId);
    
    IResource& GetResource(const std::string& resourceRelativePath);
    IResource& GetResource(const ResourceId resourceId);
    
private:
    ResourceManager(const std::string& rootResourceDirectory);
    
    void MapResourceExtensionsToLoaders();
    void LoadResourceInternal(const std::string& resourceRelativePath, const ResourceId resourceId);
    
    std::unordered_map<ResourceId, std::unique_ptr<IResource>> mResourceMap;
    std::map<std::string, IResourceLoader*> mResourceExtensionsToLoadersMap;
    
    const std::string mRootResourceDirectory;
    
    std::unique_ptr<IResourceLoader> mFileLoader;
    std::unique_ptr<IResourceLoader> mTextureLoader;
    
};

#endif /* ResourceManager_h */
