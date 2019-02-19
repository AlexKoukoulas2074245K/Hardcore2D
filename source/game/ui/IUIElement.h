//
//  IUIElement.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/02/2019.
//

#ifndef IUIElement_h
#define IUIElement_h

#include <vector>

#include "../../util/TypeTraits.h"

class IUIElement
{
public:
    virtual ~IUIElement() = default;
    IUIElement(const IUIElement&) = delete;
    IUIElement& operator = (const IUIElement&) = delete;
    
    virtual void VUpdate(const float dt) = 0;
    virtual const std::vector<EntityId>& VGetEntityIds() const = 0;
    
protected:
    IUIElement() = default;
};

#endif /* IUIElement_h */
