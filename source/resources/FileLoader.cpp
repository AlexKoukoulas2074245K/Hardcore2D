//
//  FileLoader.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "FileLoader.h"
#include "BaseResource.h"

std::unique_ptr<BaseResource> FileLoader::VCreateAndLoadResource(const std::string&, const ResourceId id)
{
    return std::unique_ptr<BaseResource>(new BaseResource(id));
}
