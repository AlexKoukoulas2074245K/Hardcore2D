//
//  TextFileLoader.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef TextFileLoader_h
#define TextFileLoader_h

#include "IResourceLoader.h"

class TextFileLoader final: public IResourceLoader
{
    friend class ResourceManager;
public:
    bool Initialize() override;
    std::unique_ptr<IResource> VCreateAndLoadResource(const std::string& path, const ResourceId id) override;
    
private:
    TextFileLoader() = default;
};

#endif /* TextFileLoader_h */
