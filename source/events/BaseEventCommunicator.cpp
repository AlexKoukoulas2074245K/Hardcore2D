//
//  BaseEventCommunicator.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#include "BaseEventCommunicator.h"
#include "EventCommunicationService.h"

BaseEventCommunicator::BaseEventCommunicator(EventCommunicationService& eventCommunicationService)
    : mEventCommunicationService(eventCommunicationService)
{
}

BaseEventCommunicator::~BaseEventCommunicator()
{
    mEventCommunicationService.UnregisterAllCallbacksForListener(this);
}
