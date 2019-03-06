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
    
    AnimationComponent(const std::string& relativeEntityAnimationsDirectoryPath, const float animationFrameDuration, ResourceManager&);
    AnimationComponent(const AnimationsMap& userSuppliedAnimations, const float animationFrameDuration);

    const std::string& GetRootAnimationsPath() const;

    FacingDirection GetCurrentFacingDirection() const;
    StringId GetCurrentAnimation() const;
    GLuint GetCurrentFrameResourceId() const;
    float GetAnimationFrameDuration() const;
    float GetAnimationTimer() const;
    const glm::vec2& GetSpecificAnimationDisplacement(const StringId animation) const;
    bool HasAnimation(const StringId animation);

    void SetFacingDirection(const FacingDirection);
    void PlayAnimation(const StringId newAnimation, const bool loop = false, const bool resetToIdleWhenFinished = true, const AnimationPriority priority = AnimationPriority::NORMAL, AnimationCompleteCallback animationCompleteCallback = nullptr);    
    void SetAnimationTimer(const float animationTimer);
    void SetSpecificAnimationDisplacement(const StringId animation, const glm::vec2& displacement);
    
    void AdvanceFrame();
    void SetPause(const bool paused);
    
private:
    void CreateAnimationsMapFromRelativeEntityAnimationsDirectory(const std::string& relativeEntityAnimationsDirectoryPath);
    void InitializeAnimationsDisplacements();
    
    ResourceManager* mResourceManager;
    
    const std::string mRootAnimationsPath;

    AnimationsMap mAnimations;
    AnimationsDisplacementMap mAnimationsDisplacements;
    
    FacingDirection mFacingDirection;
    StringId mCurrentAnimation;
    
    bool mIsLooping;
    bool mIsPaused;
    bool mResetToIdleWhenFinished;

    AnimationPriority mCurrentAnimationPriority;
    int mCurrentFrameIndex;
    float mAnimationFrameDuration;
    float mAnimationTimer;

    AnimationCompleteCallback mAnimationCompleteCallback;
};

#endif /* AnimationComponent_h */
