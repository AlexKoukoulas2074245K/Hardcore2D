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

class BaseResource;
class IResourceLoader
{
public:
    virtual ~IResourceLoader() = default;
    virtual std::unique_ptr<BaseResource> VCreateAndLoadResource(const std::string& path, const ResourceId id) = 0;
    
};

#endif /* IResourceLoader.h */
