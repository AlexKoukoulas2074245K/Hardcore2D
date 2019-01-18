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
