//
//  ShaderComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#include "ShaderComponent.h"

ShaderComponent::ShaderComponent(const StringId& shaderName, const bool affectedByPostProcessing /* true */)
    : mShaderName(shaderName)
    , mAffectedByPostProcessing(affectedByPostProcessing)
{
    
}

const StringId& ShaderComponent::GetShaderName() const
{
    return mShaderName;
}

bool ShaderComponent::IsAffectedByPostProcessing() const
{
    return mAffectedByPostProcessing;
}

void ShaderComponent::SetIsAffectedByPostProcessing(const bool affectedByPostProcessing)
{
    mAffectedByPostProcessing = affectedByPostProcessing;
}
