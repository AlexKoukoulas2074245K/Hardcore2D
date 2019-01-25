//
//  AnimationComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#include "AnimationComponent.h"
#include "../util/Logging.h"

AnimationComponent::AnimationComponent(const std::map<StringId, std::vector<GLuint>>& animations, const float animationDuration)
    : mAnimations(animations)
    , mFacingDirection(FacingDirection::RIGHT)
    , mCurrentAnimation(animations.begin()->first)
    , mCurrentFrameIndex(0)
    , mAnimationDuration(animationDuration)
    , mAnimationTimer(0.0f)
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

AnimationComponent::FacingDirection AnimationComponent::GetCurrentFacingDirection() const
{
    return mFacingDirection;
}

StringId AnimationComponent::GetCurrentAnimation() const
{
    return mCurrentAnimation;
}

GLuint AnimationComponent::GetCurrentFrameResourceId() const
{
    return mAnimations.at(mCurrentAnimation)[mCurrentFrameIndex];
}

float AnimationComponent::GetAnimationDuration() const
{
    return mAnimationDuration;
}

float AnimationComponent::GetAnimationTimer() const
{
    return mAnimationTimer;
}

void AnimationComponent::SetFacingDirection(const FacingDirection facingDirection)
{
    mFacingDirection = facingDirection;
}

void AnimationComponent::ChangeAnimation(const StringId newAnimation)
{
    if (mAnimations.count(newAnimation) == 0) assert(false);
    
    mCurrentAnimation = newAnimation;
    mCurrentFrameIndex = 0;
}

void AnimationComponent::SetAnimationTimer(const float animationTimer)
{
    mAnimationTimer = animationTimer;
}

void AnimationComponent::AdvanceFrame()
{
    mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mAnimations.at(mCurrentAnimation).size();
}
