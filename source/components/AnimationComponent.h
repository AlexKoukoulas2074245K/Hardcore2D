//
//  AnimationComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#ifndef AnimationComponent_h
#define AnimationComponent_h

#include "IComponent.h"
#include "../util/TypeTraits.h"
#include "../util/StringId.h"
#include "../GameConstants.h"

#include <unordered_map>
#include <vector>

class ResourceManager;

class AnimationComponent final: public IComponent
{
public:
    using AnimationsMap = std::unordered_map<StringId, std::vector<GLuint>>;
    
    AnimationComponent(const std::string& relativeEntityAnimationsDirectoryPath, const float animationDuration, ResourceManager&);
    
    std::string VSerializeToString() const override;
    bool VInitializeFromString(const std::string&) override;
    
    FacingDirection GetCurrentFacingDirection() const;
    StringId GetCurrentAnimation() const;
    GLuint GetCurrentFrameResourceId() const;
    float GetAnimationDuration() const;
    float GetAnimationTimer() const;
    
    void SetFacingDirection(const FacingDirection);
    void ChangeAnimation(const StringId newAnimation);
    void SetAnimationTimer(const float animationTimer);
    void AdvanceFrame();
    
private:
    void CreateAnimationsMapFromRelativeEntityAnimationsDirectory(const std::string& relativeEntityAnimationsDirectoryPath);
    
    ResourceManager& mResourceManager;
    
    AnimationsMap mAnimations;
    FacingDirection mFacingDirection;
    StringId mCurrentAnimation;
    int mCurrentFrameIndex;
    float mAnimationDuration;
    float mAnimationTimer;
};

#endif /* AnimationComponent_h */
