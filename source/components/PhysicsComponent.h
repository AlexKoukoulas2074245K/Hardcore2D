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
#include <glm/vec2.hpp>

class PhysicsComponent final: public IComponent
{
public:
    enum class BodyType
    {
        STATIC, KINEMATIC, DYNAMIC
    };
    
    struct Hitbox
    {
        Hitbox(const glm::vec2& centerPoint, const glm::vec2& dimensions)
            : mCenterPoint(centerPoint)
            , mDimensions(dimensions)
        {
        }

        glm::vec2 mCenterPoint;
        glm::vec2 mDimensions;
    };

    PhysicsComponent(const BodyType, const Hitbox&);
    PhysicsComponent(const BodyType, const Hitbox&, const glm::vec3& gravity, const glm::vec3& maxVelocity, const glm::vec3& minVelocity);
    std::string VSerializeToString() const override;
    bool VInitializeFromString(const std::string&) override;
        
    void SetBodyType(const BodyType);

    glm::vec3& GetVelocity();
    glm::vec3& GetGravity();
    glm::vec3& GetMaxVelocity();
    glm::vec3& GetMinVelocity();
        
    const glm::vec3& GetVelocity() const;
    const glm::vec3& GetGravity() const;
    const glm::vec3& GetMaxVelocity() const;
    const glm::vec3& GetMinVelocity() const;
    const Hitbox& GetHitBox() const;
    BodyType GetBodyType() const;

private:
    BodyType mBodyType;
    Hitbox mHitBox;
    glm::vec3 mVelocity;
    glm::vec3 mGravity;
    glm::vec3 mMaxVelocity;
    glm::vec3 mMinVelocity;
};

#endif /* PhysicsComponent_h */
