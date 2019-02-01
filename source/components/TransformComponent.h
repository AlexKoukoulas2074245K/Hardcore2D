//
//  TransformComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 15/01/2019.
//

#ifndef TransformComponent_h
#define TransformComponent_h

#include "IComponent.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class TransformComponent final: public IComponent
{
public:
    TransformComponent();
    TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);
    
    std::string VSerializeToString() const override;
    bool VInitializeFromString(const std::string&) override;
    
    const TransformComponent* GetParent() const;
    void SetParent(const TransformComponent* parent);

    glm::vec3& GetTranslation();
    glm::vec3& GetRotation();
    glm::vec3& GetScale();
    glm::vec3& GetPreviousTranslation();
    
    glm::vec3 GetWorldTranslation() const;
    glm::vec3 GetWorldRotation() const;
    
    const glm::vec3& GetTranslation() const;
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetScale() const;
    const glm::vec3& GetPreviousTranslation() const;
    
private:
    const TransformComponent* mParent;
    glm::vec3 mTranslation, mRotation, mScale, mPreviousTranslation;
};

#endif /* TransformComponent_h */
