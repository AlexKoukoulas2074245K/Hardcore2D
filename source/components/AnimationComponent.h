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

#include <unordered_map>
#include <vector>
#include <functional>

class ResourceManager;

class AnimationComponent final: public IComponent
{
public:
    using AnimationCompleteCallback = std::function<void()>;
    using AnimationsMap = std::unordered_map<StringId, std::vector<GLuint>, StringIdHasher>;
    
    AnimationComponent(const std::string& relativeEntityAnimationsDirectoryPath, const float animationDuration, ResourceManager&);
    AnimationComponent(const AnimationsMap& userSuppliedAnimations, const float animationDuration);

    const std::string& GetRootAnimationsPath() const;

    FacingDirection GetCurrentFacingDirection() const;
    StringId GetCurrentAnimation() const;
    GLuint GetCurrentFrameResourceId() const;
    float GetAnimationDuration() const;
    float GetAnimationTimer() const;
    bool HasAnimation(const StringId animation);

    void SetFacingDirection(const FacingDirection);
    void PlayAnimation(const StringId newAnimation, AnimationCompleteCallback animationCompleteCallback = nullptr);
    void PlayAnimationOnce(const StringId newAnimation, AnimationCompleteCallback animationCompleteCallback = nullptr);
    void SetAnimationTimer(const float animationTimer);
    void AdvanceFrame();
    void SetPause(const bool paused);
    
private:
    void CreateAnimationsMapFromRelativeEntityAnimationsDirectory(const std::string& relativeEntityAnimationsDirectoryPath);
    
    ResourceManager* mResourceManager;
    
    const std::string mRootAnimationsPath;

    AnimationsMap mAnimations;
    FacingDirection mFacingDirection;
    StringId mCurrentAnimation;
    StringId mPreviousAnimation;
    bool mPlayingOneTimeAnimation;
    bool mIsPaused;

    int mCurrentFrameIndex;
    float mAnimationDuration;
    float mAnimationTimer;

    AnimationCompleteCallback mAnimationCompleteCallback;
};

#endif /* AnimationComponent_h */
