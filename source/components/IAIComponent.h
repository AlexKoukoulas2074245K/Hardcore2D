//
//  IAIComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 29/01/2019.
//

#ifndef IAIComponent_h
#define IAIComponent_h

#include "IComponent.h"

class IAIComponent: public IComponent
{
public:
    virtual ~IAIComponent() = default;
    virtual void VUpdate(const float dt) = 0;
};

#endif /* AIComponent_h */
