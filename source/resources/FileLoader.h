//
//  FileLoader.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef FileLoader_h
#define FileLoader_h

#include "IResourceLoader.h"

class FileLoader final: public IResourceLoader
{
    friend class ResourceManager;
public:
    bool Initialize() override;
    std::unique_ptr<IResource> VCreateAndLoadResource(const std::string& path, const ResourceId id) override;
    
private:
    FileLoader() = default;
};

#endif /* FileLoader_h */
