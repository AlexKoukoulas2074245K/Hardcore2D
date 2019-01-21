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
    AnimationComponent(const std::map<StringId, std::vector<GLuint>>& animations);
    
    std::string VSerializeToString() const override;
    bool VInitializeFromString(const std::string&) override;
    
    GLuint GetCurrentFrameResourceId() const;
    void SetCurrentAnimation(const StringId animation);
    void AdvanceFrame();
    
private:
    const std::map<StringId, std::vector<GLuint>> mAnimations;
    StringId mCurrentAnimation;
    int mCurrentFrameIndex;
};

#endif /* AnimationComponent_h */
