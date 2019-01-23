//
//  EventCommunicator.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef EventCommunicator_h
#define EventCommunicator_h

#include <functional>
#include <memory>

class IEvent;
class EventCommunicationService;
using EventCallback = std::function<void(const IEvent&)>;

class EventCommunicator final
{
public:
    EventCommunicator(EventCommunicationService&);    
    EventCommunicator::~EventCommunicator();

    template <class EventType>
    inline void RegisterEventCallback(EventCallback eventCallback)
    {
        mEventCommunicationService.RegisterEventCallback(this, eventCallback);
    }

    void DispatchEvent(std::unique_ptr<IEvent> evt);

private:        
    EventCommunicationService& mEventCommunicationService;
};

#endif /* EventCommunicator_h */
