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
    ShaderComponent(const StringId& shaderName, const bool affectedByPostProcessing = true);

    const StringId& GetShaderName() const;
    bool IsAffectedByPostProcessing() const;
    void SetIsAffectedByPostProcessing(const bool affectedByPostProcessing);
    
private:
    const StringId mShaderName;
    bool mAffectedByPostProcessing;
};

#endif /* ShaderComponent_h */
