//
//  PlayerHealthbarUIElement.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/02/2019.
//

#ifndef PlayerHealthbarUIElement_h
#define PlayerHealthbarUIElement_h

#include "IUIElement.h"

class PlayerHealthbarUIElement final: public IUIElement
{
public:
    void VUpdate(const float dt) override;
};

#endif /* PlayerHealthbarUIElement_h */
