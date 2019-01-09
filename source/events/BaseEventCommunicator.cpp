//
//  BaseEventCommunicator.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#include "BaseEventCommunicator.h"
#include "../services/EventCommunicationService.h"

BaseEventCommunicator::BaseEventCommunicator(EventCommunicationService& eventCommunicationService)
    : mEventCommunicationService(eventCommunicationService)
{
}

BaseEventCommunicator::~BaseEventCommunicator()
{
    mEventCommunicationService.UnregisterAllCallbacksForListener(this);
}
