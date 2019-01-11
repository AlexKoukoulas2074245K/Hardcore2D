//
//  TextureLoader.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "TextureLoader.h"
#include "IResource.h"

bool TextureLoader::Initialize()
{
    return true;
}

std::unique_ptr<IResource> TextureLoader::VCreateAndLoadResource(const std::string&, const ResourceId id)
{
    return std::unique_ptr<IResource>(new IResource(id));
}
