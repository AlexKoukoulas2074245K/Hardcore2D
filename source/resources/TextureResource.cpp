//
//  TextureResource.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 13/01/2019.
//

#include "TextureResource.h"
#include "../gl/Context.h"

#include <SDL.h>

TextureResource::TextureResource(const ResourceId resourceId, SDL_Surface* sdlSurface, GLuint glTextureId)
    : IResource(resourceId)
    , mSdlSurface(sdlSurface)
    , mGLTextureId(glTextureId)
{
    
}

TextureResource::~TextureResource()
{
    SDL_FreeSurface(mSdlSurface);
    GL_CHECK(glDeleteTextures(1, &mGLTextureId));
}

GLuint TextureResource::GetGLTextureId() const
{
    return mGLTextureId;
}
