//
//  IInputActionConsumer.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/01/2019.
//

#ifndef IInputActionConsumer_h
#define IInputActionConsumer_h

#include "InputActions.h"

class IInputActionConsumer
{
public:
    virtual ~IInputActionConsumer() = default;
    IInputActionConsumer(const IInputActionConsumer&) = delete;
    const IInputActionConsumer& operator = (const IInputActionConsumer&) = delete;
    
    virtual bool VConsumeInputAction(const InputAction&) const = 0;
    
protected:
    IInputActionConsumer() = default;
};

#endif /* IInputActionConsumer_h */
