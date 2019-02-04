//
//  TransformComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 15/01/2019.
//

#ifndef TransformComponent_h
#define TransformComponent_h

#include "IComponent.h"
#include "../util/TypeTraits.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class TransformComponent final: public IComponent
{
public:
    TransformComponent();
    TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);
    
    EntityId GetParent() const;
    void SetParent(const EntityId parent, const glm::vec3& relativeTranslationToParent);
    
    glm::vec3& GetTranslation();
    glm::vec3& GetRotation();
    glm::vec3& GetScale();
    glm::vec3& GetPreviousTranslation();
    glm::vec3& GetRelativeTranslationToParent();

    const glm::vec3& GetTranslation() const;
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetScale() const;
    const glm::vec3& GetPreviousTranslation() const;
    const glm::vec3& GetRelativeTranslationToParent() const;
    
private:
    EntityId mParent;
    glm::vec3 mTranslation, mRotation, mScale, mPreviousTranslation, mRelativeTranslationToParent;
};

#endif /* TransformComponent_h */
