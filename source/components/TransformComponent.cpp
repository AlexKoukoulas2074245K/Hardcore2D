//
//  TransformComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 15/01/2019.
//

#include "TransformComponent.h"

TransformComponent::TransformComponent()
    : mParent(nullptr)
    , mTranslation(0.0f, 0.0f, 0.0f)
    , mRotation(0.0f, 0.0f, 0.0f)
    , mScale(1.0f, 1.0f, 1.0f)
    , mPreviousTranslation(0.0f, 0.0f, 0.0f)    
{
    
}

TransformComponent::TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
    : mParent(nullptr)
    , mTranslation(translation)
    , mRotation(rotation)
    , mScale(scale)
    , mPreviousTranslation(translation)
{
    
}

std::string TransformComponent::VSerializeToString() const
{
    return std::string();
}

bool TransformComponent::VInitializeFromString(const std::string&)
{
    return true;
}

const TransformComponent* TransformComponent::GetParent() const
{
    return mParent;
}

void TransformComponent::SetParent(const TransformComponent* parent)
{
    mParent = parent;
}

glm::vec3& TransformComponent::GetTranslation()
{
    return mTranslation;
}

glm::vec3& TransformComponent::GetRotation()
{
    return mRotation;
}

glm::vec3& TransformComponent::GetScale()
{
    return mScale;
}

glm::vec3& TransformComponent::GetPreviousTranslation()
{
    return mPreviousTranslation;
}

glm::vec3 TransformComponent::GetWorldTranslation() const
{
    glm::vec3 worldTranslation(mTranslation);
    if (mParent != nullptr)
    {
        worldTranslation += mParent->GetWorldTranslation();
    }
    
    return worldTranslation;
}

glm::vec3 TransformComponent::GetWorldRotation() const
{
    glm::vec3 worldRotation(mRotation);
    if (mParent != nullptr)
    {
        worldRotation += mParent->GetWorldRotation();
    }
    
    return worldRotation;
}

const glm::vec3& TransformComponent::GetTranslation() const
{
    return mTranslation;
}

const glm::vec3& TransformComponent::GetRotation() const
{
    return mRotation;
}

const glm::vec3& TransformComponent::GetScale() const
{
    return mScale;
}

const glm::vec3& TransformComponent::GetPreviousTranslation() const
{
    return mPreviousTranslation;
}
