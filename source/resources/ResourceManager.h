//
//  ResourceManager.h
//  Hardcore2D
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
    
	const std::string& GetRootResourceDirectory() const;

    ResourceId LoadResource(const std::string& resourceRelativePath, const bool async = false);
    void LoadResources(const std::vector<std::string>& resourceRelativePaths, const bool async = false);
    
    void UnloadResource(const std::string& resourceRelativePath);
    void UnloadResource(const ResourceId resourceId);
    
    IResource& GetResource(const std::string& resourceRelativePath);
    IResource& GetResource(const ResourceId resourceId);	
    
	template<class ResourceType>
	inline ResourceType& GetResource(const std::string& resourceRelativePath)
	{
		return static_cast<ResourceType&>(GetResource(resourceRelativePath));
	}

	template<class ResourceType>
	inline ResourceType& GetResource(const ResourceId resourceId)
	{
		return static_cast<ResourceType&>(GetResource(resourceId));
	}

private:
    ResourceManager(const std::string& rootResourceDirectory);
    
    void MapResourceExtensionsToLoaders();
    void LoadResourceInternal(const std::string& resourceRelativePath, const ResourceId resourceId);
    
    std::unordered_map<ResourceId, std::unique_ptr<IResource>> mResourceMap;
    std::map<std::string, IResourceLoader*> mResourceExtensionsToLoadersMap;
    
    const std::string mRootResourceDirectory;
    
    std::unique_ptr<IResourceLoader> mTextFileLoader;
    std::unique_ptr<IResourceLoader> mTextureLoader;
    
};

#endif /* ResourceManager_h */
