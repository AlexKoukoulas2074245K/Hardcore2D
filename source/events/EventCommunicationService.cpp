//
//  EventCommunicationService.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 08/01/2019.
//

#include "EventCommunicationService.h"
#include "EventCommunicator.h"
#include "../util/Logging.h"
#include "../util/StringUtils.h"
#include "IEvent.h"

EventCommunicationService::EventCommunicationService()
    : mEventDispatchingDepth(0)
{
    
}

EventCommunicationService::~EventCommunicationService()
{
}

std::unique_ptr<EventCommunicator> EventCommunicationService::CreateEventCommunicator()
{
    return std::unique_ptr<EventCommunicator>(new EventCommunicator(*this));
}

void EventCommunicationService::DispatchEvent(EventCommunicator*, std::unique_ptr<IEvent> evt)
{
    auto event = std::move(evt);
    auto& eventImpl = *(event.get());
    const auto eventTypeName = typeid(eventImpl).name();
    const auto eventTypeHash = GetStringHash(eventTypeName);
    
    // Suppress logging of collision events to avoid polluting stdout 
    if (StringEndsWith(eventTypeName, "CollisionEvent") == false)
    {
        Log(LogType::INFO, "Dispatching event: %s", eventTypeName);
    }
    
    
    mEventDispatchingDepth++;
    
    if (mEventCallbackRegistry.count(eventTypeHash) > 0)
    {
        const auto& callbackVec = mEventCallbackRegistry.at(eventTypeHash);
        for (auto& listenerCallbackPair: callbackVec)
        {
            if (listenerCallbackPair.second)
            {
                listenerCallbackPair.second(*event);
            }
        }
    }
    
    mEventDispatchingDepth--;
    
    // After a cascade of event dispatching has finished, we check whether there are
    // any pending callback registrations to be added. If not hanlded that way, callbacks
    // for event X could end up being registered while event X was being dispatched, hence
    // modifying the vector during iteration = dead
    if (mEventDispatchingDepth == 0 && mPendingCallbacksToBeRegistered.size() > 0)
    {
        for (auto& eventTypeCallbackPair: mPendingCallbacksToBeRegistered)
        {
            for (auto callback: eventTypeCallbackPair.second)
            {
                mEventCallbackRegistry[eventTypeCallbackPair.first].push_back(callback);
            }
        }
    }
    mPendingCallbacksToBeRegistered.clear();
}

void EventCommunicationService::UnregisterAllCallbacksForListener(EventCommunicator* listener)
{
    for (auto& eventTypeCallbackVecPair: mEventCallbackRegistry)
    {
        auto listenerCallbackPairIter = eventTypeCallbackVecPair.second.begin();
        while (listenerCallbackPairIter != eventTypeCallbackVecPair.second.end())
        {
            if ((*listenerCallbackPairIter).first == listener)
            {
                listenerCallbackPairIter = eventTypeCallbackVecPair.second.erase(listenerCallbackPairIter);
            }
            else
            {
                listenerCallbackPairIter++;
            }
        }
    }
}
