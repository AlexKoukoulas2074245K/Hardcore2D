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
public:
    std::unique_ptr<BaseResource> VCreateAndLoadResource(const std::string& path, const ResourceId id) override;
};

#endif /* FileLoader_h */
