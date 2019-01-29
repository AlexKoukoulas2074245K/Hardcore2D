//
//  LightSamuraiAIComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 29/01/2019.
//

#ifndef LightSamuraiAIComponent_h
#define LightSamuraiAIComponent_h

#include "IAIComponent.h"

class LightSamuraiAIComponent final: public IAIComponent
{
public:
    void VUpdate(const float dt) override;
    std::string VSerializeToString() const override;
    bool VInitializeFromString(const std::string&) override;
};

#endif /* LightSamuraiAIComponent_h */
