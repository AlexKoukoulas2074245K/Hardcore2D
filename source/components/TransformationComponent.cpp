//
//  TransformationComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 15/01/2019.
//

#include "TransformationComponent.h"

TransformationComponent::TransformationComponent()
    : mTranslation(0.0f, 0.0f, 0.0f)
    , mRotation(0.0f, 0.0f, 0.0f)
    , mScale(1.0f, 1.0f, 1.0f)
    , mPreviousTranslation(0.0f, 0.0f, 0.0f)
{
    
}

std::string TransformationComponent::VSerializeToString() const
{
    return std::string();
}

bool TransformationComponent::VInitializeFromString(const std::string&)
{
    return true;
}

glm::vec3& TransformationComponent::GetTranslation()
{
    return mTranslation;
}

glm::vec3& TransformationComponent::GetRotation()
{
    return mRotation;
}

glm::vec3& TransformationComponent::GetScale()
{
    return mScale;
}

glm::vec3& TransformationComponent::GetPreviousTranslation()
{
    return mPreviousTranslation;
}

const glm::vec3& TransformationComponent::GetTranslation() const
{
    return mTranslation;
}

const glm::vec3& TransformationComponent::GetRotation() const
{
    return mRotation;
}

const glm::vec3& TransformationComponent::GetScale() const
{
    return mScale;
}

const glm::vec3& TransformationComponent::GetPreviousTranslation() const
{
    return mPreviousTranslation;
}
