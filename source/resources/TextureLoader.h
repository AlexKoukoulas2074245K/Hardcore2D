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
    friend class ResourceManager;
public:
    bool Initialize() override;
    std::unique_ptr<IResource> VCreateAndLoadResource(const std::string& path) override;
private:
    TextureLoader() = default;
};


#endif /* TextureLoader_h */
