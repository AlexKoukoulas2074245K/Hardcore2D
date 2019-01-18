//
//  Shader.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 15/01/2019.
//

#include "Shader.h"

Shader::Shader(const GLuint shaderId, const std::map<std::string, GLuint> uniformNamesToLocations)
    : mShaderId(shaderId)
    , mShaderUniformNamesToLocations(uniformNamesToLocations)
{
    
}

GLuint Shader::GetShaderId() const
{
    return mShaderId;
}

const std::map<std::string, GLuint>& Shader::GetUniformNamesToLocations() const
{
    return mShaderUniformNamesToLocations;
}
