//
//  ShaderComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#ifndef ShaderComponent_h
#define ShaderComponent_h

#include "IComponent.h"
#include "../util/StringId.h"

class ShaderComponent final: public IComponent
{
public:
    ShaderComponent(const StringId& shaderName);

    const StringId& GetShaderName() const;
    
private:
    const StringId mShaderName;
};

#endif /* ShaderComponent_h */
