//
//  AnimationComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(const std::map<StringId, std::vector<GLuint>>& animations)
    : mAnimations(animations)
    , mCurrentAnimation(animations.begin()->first)
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
    return mAnimations.at(mCurrentAnimation)[mCurrentFrameIndex];
}

void AnimationComponent::SetCurrentAnimation(const StringId animation)
{
    mCurrentAnimation = animation;
}

void AnimationComponent::AdvanceFrame()
{
    mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mAnimations.at(mCurrentAnimation).size();
}
