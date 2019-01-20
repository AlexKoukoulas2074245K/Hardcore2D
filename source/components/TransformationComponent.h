//
//  TransformationComponent.h
//  Hardcore2D
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
    
    glm::vec3& GetTranslation();
    glm::vec3& GetRotation();
    glm::vec3& GetScale();
    
    const glm::vec3& GetTranslation() const;
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetScale() const;
    
private:
    glm::vec3 mTranslation, mRotation, mScale;
};

#endif /* TransformationComponent_h */
