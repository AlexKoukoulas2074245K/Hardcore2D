//
//  ShaderComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#include "ShaderComponent.h"

ShaderComponent::ShaderComponent(const StringId& shaderName)
    : mShaderName(shaderName)
{
    
}

const StringId& ShaderComponent::GetShaderName() const
{
    return mShaderName;
}
