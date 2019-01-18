//
//  ShaderComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#include "ShaderComponent.h"

ShaderComponent::ShaderComponent(const std::string& shaderName)
    : mShaderName(shaderName)
{
    
}

std::string ShaderComponent::VSerializeToString() const
{
    return std::string();
}

bool ShaderComponent::VInitializeFromString(const std::string&)
{
    return true;
}

const std::string& ShaderComponent::GetShaderName() const
{
    return mShaderName;
}
