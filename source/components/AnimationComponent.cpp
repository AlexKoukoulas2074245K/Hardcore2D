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

AnimationComponent::AnimationComponent(const std::string& relativeEntityAnimationsDirectoryPath, const float animationFrameDuration, ResourceManager& resourceManager)
    : mResourceManager(&resourceManager)
    , mRootAnimationsPath(relativeEntityAnimationsDirectoryPath)
    , mFacingDirection(FacingDirection::RIGHT)
    , mCurrentAnimation("")    
    , mIsLooping(false)
    , mIsPaused(false)
    , mResetToIdleWhenFinished(true)
    , mCurrentAnimationPriority(AnimationPriority::NORMAL)
    , mCurrentFrameIndex(0)
    , mAnimationFrameDuration(animationFrameDuration)
    , mAnimationTimer(0.0f)
{
    CreateAnimationsMapFromRelativeEntityAnimationsDirectory(relativeEntityAnimationsDirectoryPath);
    InitializeAnimationsDisplacements();
}

AnimationComponent::AnimationComponent(const AnimationsMap& userSuppliedAnimations, const float animationFrameDuration)
    : mResourceManager(nullptr)
    , mRootAnimationsPath("")
    , mAnimations(userSuppliedAnimations)
    , mFacingDirection(FacingDirection::RIGHT)
    , mCurrentAnimation("")    
    , mIsLooping(false)
    , mIsPaused(false)
    , mResetToIdleWhenFinished(true)
    , mCurrentAnimationPriority(AnimationPriority::NORMAL)
    , mCurrentFrameIndex(0)
    , mAnimationFrameDuration(animationFrameDuration)
    , mAnimationTimer(0.0f)
{
    PlayAnimation(StringId("idle"), true);
    InitializeAnimationsDisplacements();
}

const std::string& AnimationComponent::GetRootAnimationsPath() const
{
    return mRootAnimationsPath;
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

float AnimationComponent::GetAnimationFrameDuration() const
{
    return mAnimationFrameDuration;
}

float AnimationComponent::GetAnimationTimer() const
{
    return mAnimationTimer;
}

const glm::vec2& AnimationComponent::GetSpecificAnimationDisplacement(const StringId animation) const
{
    return mAnimationsDisplacements.at(animation);
}

bool AnimationComponent::HasAnimation(const StringId animation)
{
    return mAnimations.count(animation) != 0;
}

void AnimationComponent::SetFacingDirection(const FacingDirection facingDirection)
{
    mFacingDirection = facingDirection;
}

void AnimationComponent::PlayAnimation(const StringId newAnimation, const bool loop /* false */, const bool resetToIdleWhenFinished /* true */, const AnimationPriority priority /* AnimationPriority::NORMAL */, AnimationCompleteCallback animationCompleteCallback /* nullptr */)
{
    if (mAnimations.count(newAnimation) == 0)
    {
        assert(false);
    }
    
    if (animationCompleteCallback != nullptr)
    {
        mAnimationCompleteCallback = animationCompleteCallback;
    }

    if (mCurrentAnimation == newAnimation)
    {
        return;
    }

    // Only override current animation if the new one is of the same or higher priority
    if (static_cast<int>(priority) >= static_cast<int>(mCurrentAnimationPriority))
    {
        mIsPaused = false;
        mResetToIdleWhenFinished = resetToIdleWhenFinished;
        mIsLooping = loop;
        mCurrentAnimation = newAnimation;
        mCurrentAnimationPriority = priority;
        mCurrentFrameIndex = 0;
    }    
}

void AnimationComponent::SetAnimationTimer(const float animationTimer)
{
    mAnimationTimer = animationTimer;
}

void AnimationComponent::SetSpecificAnimationDisplacement(const StringId animation, const glm::vec2& displacement)
{
    mAnimationsDisplacements[animation] = displacement;
}

void AnimationComponent::AdvanceFrame()
{
    if (mIsPaused)
    {
        return;
    }

    mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mAnimations.at(mCurrentAnimation).size();
    if (mCurrentFrameIndex == 0)
    {                        
        if (mIsLooping)
        {
            mCurrentFrameIndex = 0;
        }
        else
        {                                  
            if (mResetToIdleWhenFinished)
            {
                mCurrentAnimation = StringId("idle");
                mCurrentFrameIndex = 0;
            }      
            else
            {
                mCurrentFrameIndex = mAnimations.at(mCurrentAnimation).size() - 1;
                mIsPaused = true;
            }
        }

        if (mAnimationCompleteCallback != nullptr)
        {
            mAnimationCompleteCallback();
        }

        // Reset animation priority so that subsequent animations can be played if the current one 
        // which just finished was high priority. This means that a high priority animation des not make sense to be looping
        mCurrentAnimationPriority = AnimationPriority::NORMAL;
    }
}

void AnimationComponent::SetPause(const bool paused)
{
    mIsPaused = paused;
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
    
    PlayAnimation(StringId("idle"), true);
}

void AnimationComponent::InitializeAnimationsDisplacements()
{
    for (const auto animation: mAnimations)
    {
        mAnimationsDisplacements[animation.first] = glm::vec2(0.0f, 0.0f);
    }
}
