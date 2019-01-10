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

class BaseResource;
class IResourceLoader;
class ResourceManager final: public IService
{
public:
    ResourceManager();
    ~ResourceManager();
    
    void LoadResource(const std::string& resourcePath, const bool async = false);
    void LoadResources(const std::vector<const std::string>& resourcePaths, const bool async = false);
    
    void UnloadResource(const BaseResource& baseResource);
    void UnloadResource(const ResourceId resourceId);
    
    BaseResource& GetResource(const std::string& resourcePath);
    BaseResource& GetResource(const ResourceId resourceId);
    
private:
    void MapResourceExtensionsToLoaders();
    void LoadResourceInternal(const std::string& resourcePath, const ResourceId resourceId);
    
    std::unordered_map<ResourceId, std::unique_ptr<BaseResource>> mResourceMap;
    std::map<std::string, std::shared_ptr<IResourceLoader>> mResourceExtensionsToLoadersMap;
};

#endif /* ResourceManager_h */
