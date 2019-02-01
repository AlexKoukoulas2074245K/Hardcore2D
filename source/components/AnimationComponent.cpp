//
//  AnimationComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#include "AnimationComponent.h"
#include "../util/Logging.h"
#include "../util/FileUtils.h"
#include "../resources/ResourceManager.h"
#include "../resources/TextureResource.h"

#include <cassert>

AnimationComponent::AnimationComponent(const std::string& relativeEntityAnimationsDirectoryPath, const float animationDuration, ResourceManager& resourceManager)
    : mResourceManager(&resourceManager)
    , mFacingDirection(FacingDirection::RIGHT)
    , mCurrentAnimation("")
    , mPreviousAnimation("")
    , mPlayingOneTimeAnimation(false)
    , mCurrentFrameIndex(0)
    , mAnimationDuration(animationDuration)
    , mAnimationTimer(0.0f)
{
    CreateAnimationsMapFromRelativeEntityAnimationsDirectory(relativeEntityAnimationsDirectoryPath);
}

AnimationComponent::AnimationComponent(const AnimationsMap& userSuppliedAnimations, const float animationDuration)
    : mResourceManager(nullptr)
    , mAnimations(userSuppliedAnimations)
    , mFacingDirection(FacingDirection::RIGHT)
    , mCurrentAnimation("")
    , mPreviousAnimation("")
    , mPlayingOneTimeAnimation(false)
    , mCurrentFrameIndex(0)
    , mAnimationDuration(animationDuration)
    , mAnimationTimer(0.0f)
{
    PlayAnimation(StringId("idle"));
}

std::string AnimationComponent::VSerializeToString() const
{
    return std::string();
}

bool AnimationComponent::VInitializeFromString(const std::string&)
{
    return true;
}

FacingDirection AnimationComponent::GetCurrentFacingDirection() const
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

void AnimationComponent::PlayAnimation(const StringId newAnimation)
{
    if (mAnimations.count(newAnimation) == 0) assert(false);    
    if (mCurrentAnimation == newAnimation)
    {
        return;
    }
    if (mPlayingOneTimeAnimation)
    {
        mPreviousAnimation = newAnimation;
        return;
    }

    mCurrentAnimation = newAnimation;    
    mCurrentFrameIndex = 0;
}

void AnimationComponent::PlayAnimationOnce(const StringId newAnimation)
{
    if (mAnimations.count(newAnimation) == 0) assert(false);
    if (mCurrentAnimation == newAnimation)
    {
        return;
    }
    if (mPlayingOneTimeAnimation)
    {
        mPreviousAnimation = newAnimation;
        return;
    }

    mPlayingOneTimeAnimation = true;
    mPreviousAnimation = mCurrentAnimation;
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
    if (mCurrentFrameIndex == 0 && mPlayingOneTimeAnimation)
    {
        mPlayingOneTimeAnimation = false;
        PlayAnimation(mPreviousAnimation);
    }
}

void AnimationComponent::CreateAnimationsMapFromRelativeEntityAnimationsDirectory(const std::string& relativeEntityAnimationsDirectoryPath)
{
    AnimationsMap result;
    const auto rootEntityAnimationsDirectory = ResourceManager::RES_ROOT + relativeEntityAnimationsDirectoryPath;
    auto animationFolderNames = GetAllFilenamesInDirectory(rootEntityAnimationsDirectory);
    
    for (const auto animationName : animationFolderNames)
    {
        const auto animationNameStringId = StringId(animationName);
        const auto relativeAnimationDirectoryPath = relativeEntityAnimationsDirectoryPath + "/" + animationName;
        const auto animationFrameFiles = GetAllFilenamesInDirectory(ResourceManager::RES_ROOT + relativeAnimationDirectoryPath);
        
        for (const auto animationFrameNumber: animationFrameFiles)
        {
            auto resourceId = mResourceManager->LoadResource(relativeAnimationDirectoryPath + "/" + animationFrameNumber);
            mAnimations[animationNameStringId].push_back(mResourceManager->GetResource<TextureResource>(resourceId).GetGLTextureId());
        }
        
    }
    
    PlayAnimation(StringId("idle"));
}

