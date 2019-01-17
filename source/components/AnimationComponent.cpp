//
//  AnimationComponent.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(const std::vector<GLuint>& frameTextureIds)
    : mFrameTextureIds(frameTextureIds)
    , mCurrentFrameIndex(0)
{
}

std::string AnimationComponent::VSerializeToString() const
{
    return std::string();
}

bool AnimationComponent::VInitializeFromString(const std::string&)
{
    return true;
}

GLuint AnimationComponent::GetCurrentFrameResourceId() const
{
    return mFrameTextureIds[mCurrentFrameIndex];
}

void AnimationComponent::AdvanceFrame()
{
    mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mFrameTextureIds.size();
}
