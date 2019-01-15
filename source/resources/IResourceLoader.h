//
//  IResourceLoader.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef IResourceLoader_h
#define IResourceLoader_h

#include "../util/TypeTraits.h"

#include <memory>
#include <string>

class IResource;
class IResourceLoader
{
public:
    virtual ~IResourceLoader() = default;
    IResourceLoader(const IResourceLoader&) = delete;
    const IResourceLoader& operator = (const IResourceLoader&) = delete;
    
    virtual bool Initialize() = 0;
    
    virtual std::unique_ptr<IResource> VCreateAndLoadResource(const std::string& path, const ResourceId id) = 0;

protected:
    IResourceLoader() = default;
};

#endif /* IResourceLoader.h */
