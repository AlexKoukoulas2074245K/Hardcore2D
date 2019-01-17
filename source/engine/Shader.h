//
//  Shader.h
//  Ninja
//
//  Created by Alex Koukoulas on 15/01/2019.
//

#ifndef Shader_h
#define Shader_h

#include "../util/TypeTraits.h"

#include <string>
#include <map>

class Shader final
{
public:
    Shader(const GLuint shaderId, const std::map<std::string, GLuint> uniformNamesToLocations);
    
    GLuint GetShaderId() const;
    const std::map<std::string, GLuint>& GetUniformNamesToLocations() const;
    
private:
    GLuint mShaderId;
    std::map<std::string, GLuint> mShaderUniformNamesToLocations;
};

#endif /* Shader_h */
