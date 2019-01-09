//
//  BaseEventCommunicator.h
//  Ninja
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef BaseEventCommunicator_h
#define BaseEventCommunicator_h

class EventCommunicationService;
class BaseEventCommunicator
{
public:
    BaseEventCommunicator(EventCommunicationService&);
    virtual ~BaseEventCommunicator();
    
private:
    EventCommunicationService& mEventCommunicationService;
};

#endif /* BaseEventCommunicator_h */
