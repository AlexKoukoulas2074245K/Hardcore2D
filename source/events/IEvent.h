//
//  IEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef IEvent_h
#define IEvent_h

class IEvent
{
public:
    virtual ~IEvent() = default;
    IEvent(const IEvent&) = delete;
    const IEvent& operator = (const IEvent&) = delete;
    
protected:
    IEvent() = default;
};

#endif /* IEvent_h */
