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

#include <map>
#include <vector>

class AnimationComponent final: public IComponent
{
public:
    enum class FacingDirection
    {
        LEFT, RIGHT
    };
    
    AnimationComponent(const std::map<StringId, std::vector<GLuint>>& animations, const float animationDuration);
    
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
    const std::map<StringId, std::vector<GLuint>> mAnimations;
    FacingDirection mFacingDirection;
    StringId mCurrentAnimation;
    int mCurrentFrameIndex;
    float mAnimationDuration;
    float mAnimationTimer;
};

#endif /* AnimationComponent_h */
