//
//  TextureLoader.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "TextureLoader.h"
#include "BaseResource.h"

std::unique_ptr<BaseResource> TextureLoader::VCreateAndLoadResource(const std::string&, const ResourceId id)
{
    return std::unique_ptr<BaseResource>(new BaseResource(id));
}
