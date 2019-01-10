//
//  TextureLoader.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef TextureLoader_h
#define TextureLoader_h

#include "IResourceLoader.h"

class TextureLoader final: public IResourceLoader
{
public:
    std::unique_ptr<BaseResource> VCreateAndLoadResource(const std::string& path, const ResourceId id) override;
};


#endif /* TextureLoader_h */
