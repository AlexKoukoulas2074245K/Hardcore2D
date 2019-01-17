//
//  AnimationComponent.h
//  Ninja
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#ifndef AnimationComponent_h
#define AnimationComponent_h

#include "IComponent.h"
#include "../util/TypeTraits.h"

#include <vector>

class AnimationComponent final: public IComponent
{
public:
    AnimationComponent(const std::vector<GLuint>& frameTextureIds);
    
    std::string VSerializeToString() const override;
    bool VInitializeFromString(const std::string&) override;
    
    GLuint GetCurrentFrameResourceId() const;
    void AdvanceFrame();
    
private:
    const std::vector<GLuint> mFrameTextureIds;
    int mCurrentFrameIndex;
};

#endif /* AnimationComponent_h */
