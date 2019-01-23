//
//  DebugInputActionConsumer.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/01/2019.
//

#ifndef DebugInputActionConsumer_h
#define DebugInputActionConsumer_h

#include "IInputActionConsumer.h"

class DebugInputActionConsumer final: public IInputActionConsumer
{
public:

    virtual bool VConsumeInputAction(const InputAction&) const;
};

#endif /* DebugInputActionConsumer_h */
