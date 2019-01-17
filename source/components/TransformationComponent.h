//
//  TransformationComponent.h
//  Ninja
//
//  Created by Alex Koukoulas on 15/01/2019.
//

#ifndef TransformationComponent_h
#define TransformationComponent_h

#include "IComponent.h"

#include <glm/vec3.hpp>

class TransformationComponent final: public IComponent
{
public:
    TransformationComponent();
    
    std::string VSerializeToString() const override;
    bool VInitializeFromString(const std::string&) override;
    
    glm::vec3 mTranslation, mRotation, mScale;
};

#endif /* TransformationComponent_h */
