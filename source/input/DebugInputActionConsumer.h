//
//  DebugInputActionConsumer.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 22/01/2019.
//

#ifndef DebugInputActionConsumer_h
#define DebugInputActionConsumer_h

#include "IInputActionConsumer.h"

#include <memory>

class EventCommunicator;
class ServiceLocator;

class DebugInputActionConsumer final: public IInputActionConsumer
{
public:
    DebugInputActionConsumer(const ServiceLocator&);
    ~DebugInputActionConsumer();

    virtual bool VConsumeInputAction(const InputAction&) const;

private:
    std::unique_ptr<EventCommunicator> mEventCommunicator;
};

#endif /* DebugInputActionConsumer_h */
