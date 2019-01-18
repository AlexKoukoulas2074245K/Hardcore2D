//
//  ShaderComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 16/01/2019.
//

#ifndef ShaderComponent_h
#define ShaderComponent_h

#include "IComponent.h"

class ShaderComponent final: public IComponent
{
public:
    ShaderComponent(const std::string& shaderName);
    
    std::string VSerializeToString() const override;
    bool VInitializeFromString(const std::string&) override;

    const std::string& GetShaderName() const;
    
private:
    const std::string mShaderName;
};

#endif /* ShaderComponent_h */
