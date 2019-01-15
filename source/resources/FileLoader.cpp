//
//  FileLoader.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "FileLoader.h"
#include "IResource.h"

bool FileLoader::Initialize()
{
    return true;
}

std::unique_ptr<IResource> FileLoader::VCreateAndLoadResource(const std::string&, const ResourceId id)
{
    return std::unique_ptr<IResource>(new IResource(id));
}
