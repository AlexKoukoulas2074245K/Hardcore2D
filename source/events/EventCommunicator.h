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
    friend class EventCommunicationService;

public:      
    EventCommunicator::~EventCommunicator();
    EventCommunicator(const EventCommunicator&) = delete;
    const EventCommunicator& operator = (const EventCommunicator&) = delete;
   
    template <class EventType>
    inline void RegisterEventCallback(EventCallback eventCallback)
    {
        mEventCommunicationService.RegisterEventCallback<EventType>(this, eventCallback);
    }

    void DispatchEvent(std::unique_ptr<IEvent> evt);

private:        
    EventCommunicator(EventCommunicationService&);

    EventCommunicationService& mEventCommunicationService;
};

#endif /* EventCommunicator_h */
