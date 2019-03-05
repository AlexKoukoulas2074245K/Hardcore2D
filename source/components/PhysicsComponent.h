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
    friend class SetEntityVelocityCommand;
    friend class SetEntityVelocityAndAnimateCommand;
    friend class PhysicsSystem;
    friend class SetAngularVelocityCommand;
    
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
    
    void SetPushbackForce(const glm::vec3& pushbackForce);
    void SetBodyType(const BodyType);
    void ScaleHitBoxBy(const float scaleMultiplier);
    void SetMaxVelocity(const glm::vec3& maxVelocity);
    void SetMinVelocity(const glm::vec3& minVelocity);

    const glm::vec3& GetVelocity() const;
    const glm::vec3& GetGravity() const;
    const glm::vec3& GetMaxVelocity() const;
    const glm::vec3& GetMinVelocity() const;    
    float GetAngularVelocity() const;
    const Hitbox& GetHitBox() const;
    BodyType GetBodyType() const;

private:
    BodyType mBodyType;
    Hitbox mHitBox;
    glm::vec3 mVelocity;
    glm::vec3 mGravity;
    glm::vec3 mMaxVelocity;
    glm::vec3 mMinVelocity;
    glm::vec3 mPushbackForce;
    float mAngularVelocity;
};

#endif /* PhysicsComponent_h */
