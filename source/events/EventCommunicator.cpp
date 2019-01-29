//
//  EventCommunicator.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#include "EventCommunicator.h"
#include "IEvent.h"

EventCommunicator::EventCommunicator(EventCommunicationService& eventCommunicationService)
    : mEventCommunicationService(eventCommunicationService)
{
}

EventCommunicator::~EventCommunicator()
{
    mEventCommunicationService.UnregisterAllCallbacksForListener(this);
}

void EventCommunicator::DispatchEvent(std::unique_ptr<IEvent> evt)
{
    mEventCommunicationService.DispatchEvent(this, std::move(evt));
}
