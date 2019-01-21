//
//  PhysicsComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#ifndef PhysicsComponent_h
#define PhysicsComponent_h

#include "IComponent.h"
#include <glm/vec3.hpp>

class PhysicsComponent final: public IComponent
{
public:
    enum class BodyType
    {
        STATIC, KINEMATIC, DYNAMIC
    };
    
    PhysicsComponent(const BodyType);
    
    std::string VSerializeToString() const override;
    bool VInitializeFromString(const std::string&) override;
    
    BodyType GetBodyType() const;
    void SetBodyType(const BodyType);
    
    glm::vec3& GetVelocity();
    glm::vec3& GetGravity();
    glm::vec3& GetMaxVelocity();
    glm::vec3& GetMinVelocity();
    
    const glm::vec3& GetVelocity() const;
    const glm::vec3& GetGravity() const;
    const glm::vec3& GetMaxVelocity() const;
    const glm::vec3& GetMinVelocity() const;
    
private:
    BodyType mBodyType;
    glm::vec3 mVelocity;
    glm::vec3 mGravity;
    glm::vec3 mMaxVelocity;
    glm::vec3 mMinVelocity;
};

#endif /* PhysicsComponent_h */
