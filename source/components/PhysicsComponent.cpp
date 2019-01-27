//
//  PhysicsComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 18/01/2019.
//

#include "PhysicsComponent.h"

#include "../util/TypeTraits.h"

PhysicsComponent::PhysicsComponent(const BodyType bodyType, const Hitbox& hitBox)
    : mBodyType(bodyType)
    , mHitBox(hitBox)
    , mVelocity(0.0f, 0.0f, 0.0f)
    , mGravity(0.0f, 0.0f, 0.0f)
    , mMaxVelocity(FLT_MAX, FLT_MAX, 0.0f)
    , mMinVelocity(-FLT_MAX, -FLT_MAX, 0.0f)
{
    
}

PhysicsComponent::PhysicsComponent(const BodyType bodyType, const Hitbox& hitBox, const glm::vec3& gravity, const glm::vec3& maxVelocity, const glm::vec3& minVelocity)
    : mBodyType(bodyType)
    , mHitBox(hitBox)
    , mGravity(gravity)
    , mMaxVelocity(maxVelocity)
    , mMinVelocity(minVelocity)
{
    
}

std::string PhysicsComponent::VSerializeToString() const
{
    return std::string();
}

bool PhysicsComponent::VInitializeFromString(const std::string&)
{
    return true;
}

void PhysicsComponent::SetBodyType(const BodyType bodyType)
{
    mBodyType = bodyType;
}

glm::vec3& PhysicsComponent::GetVelocity()
{
    return mVelocity;
}

glm::vec3& PhysicsComponent::GetGravity()
{
    return mGravity;
}

glm::vec3& PhysicsComponent::GetMaxVelocity()
{
    return mMaxVelocity;
}

glm::vec3& PhysicsComponent::GetMinVelocity()
{
    return mMinVelocity;
}

const PhysicsComponent::Hitbox& PhysicsComponent::GetHitBox() const
{
    return mHitBox;
}

const glm::vec3& PhysicsComponent::GetVelocity() const
{
    return mVelocity;
}
const glm::vec3& PhysicsComponent::GetGravity() const
{
    return mGravity;
}

const glm::vec3& PhysicsComponent::GetMaxVelocity() const
{
    return mMaxVelocity;
}

const glm::vec3& PhysicsComponent::GetMinVelocity() const
{
    return mMinVelocity;
}

PhysicsComponent::BodyType PhysicsComponent::GetBodyType() const
{
    return mBodyType;
}
