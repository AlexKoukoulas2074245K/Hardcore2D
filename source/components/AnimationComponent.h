//
//  AnimationComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#ifndef AnimationComponent_h
#define AnimationComponent_h

#include "IComponent.h"
#include "../util/StringId.h"
#include "../game/GameTypeTraits.h"
#include "../game/GameConstants.h"
#include "../util/MathUtils.h"

#include <unordered_map>
#include <vector>
#include <functional>

class ResourceManager;

class AnimationComponent final: public IComponent
{
public:
    enum class AnimationPriority
    {
        NORMAL = 0, HIGH = 1
    };

    using AnimationCompleteCallback = std::function<void()>;
    using AnimationsMap = std::unordered_map<StringId, std::vector<GLuint>, StringIdHasher>;
    using AnimationsDisplacementMap = std::unordered_map<StringId, glm::vec2, StringIdHasher>;
    using AnimationsFrameDurationsMap = std::unordered_map<StringId, float, StringIdHasher>;
    
    AnimationComponent(const std::string& relativeEntityAnimationsDirectoryPath, const float defaultAnimationFrameDuration, ResourceManager&, const bool idleAnimationLoop = false, const bool idleAnimationResetToIdleWhenFinished = true);
    AnimationComponent(const AnimationsMap& userSuppliedAnimations, const float defaultAnimationFrameDuration, const bool idleAnimationLoop = false, const bool idleAnimationResetToIdleWhenFinished = true);

    const std::string& GetRootAnimationsPath() const;

    FacingDirection GetCurrentFacingDirection() const;
    StringId GetCurrentAnimation() const;
    GLuint GetCurrentFrameResourceId() const;
    float GetCurrentAnimationFrameDuration() const;
    float GetAnimationTimer() const;
    const glm::vec2& GetSpecificAnimationDisplacement(const StringId animation) const;
    bool HasAnimation(const StringId animation);

    void SetFacingDirection(const FacingDirection);
    void PlayAnimation(const StringId newAnimation, const bool loop = false, const bool resetToIdleWhenFinished = true, const AnimationPriority priority = AnimationPriority::NORMAL, AnimationCompleteCallback animationCompleteCallback = nullptr);    
    void SetAnimationTimer(const float animationTimer);
    void SetSpecificAnimationDisplacement(const StringId animation, const glm::vec2& displacement);
    void SetSpecificAnimationFrameDuration(const StringId animation, const float animationFrameDuration);
    
    void AdvanceFrame();
    void SetPause(const bool paused);
    
private:
    void CreateAnimationsMapFromRelativeEntityAnimationsDirectory(const std::string& relativeEntityAnimationsDirectoryPath);
    void InitializeAnimationsDisplacements();
    void InitializeAnimationsFrameDurations(const float defaultAnimationFrameDuration);
    
    ResourceManager* mResourceManager;
    
    const std::string mRootAnimationsPath;

    AnimationsMap mAnimations;
    AnimationsDisplacementMap mAnimationsDisplacements;
    AnimationsFrameDurationsMap mAnimationsFrameDurations;
    
    FacingDirection mFacingDirection;
    StringId mCurrentAnimation;
    
    bool mIsLooping;
    bool mIsPaused;
    bool mResetToIdleWhenFinished;

    AnimationPriority mCurrentAnimationPriority;
    int mCurrentFrameIndex;
    float mAnimationTimer;

    AnimationCompleteCallback mAnimationCompleteCallback;
};

#endif /* AnimationComponent_h */
