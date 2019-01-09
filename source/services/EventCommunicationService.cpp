//
//  EventCommunicationService.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 08/01/2019.
//

#include "EventCommunicationService.h"

#if defined(DEBUG) || defined(_DEBUG)
#include <iostream>
#endif

void EventCommunicationService::DispatchEvent(BaseEventCommunicator* dispatcher, std::unique_ptr<IEvent> evt)
{
    auto event = std::move(evt);
    auto& eventImpl = *(event.get());
    const auto eventTypeName = typeid(eventImpl).name();
    const auto eventTypeHash = GetTypeHash(eventTypeName);
    
#ifdef DEBUG
    std::cout << "Dispatching event: " << eventTypeName << std::endl;
#endif
    
    mEventDispatchingHistory.emplace_back(std::make_pair(eventTypeName, dispatcher));
    
    if (mEventCallbackRegistry.count(eventTypeHash) > 0)
    {
        const auto& callbackVec = mEventCallbackRegistry.at(eventTypeHash);
        for (auto& listenerCallbackPair: callbackVec)
        {
            listenerCallbackPair.second(*event);
        }
    }
}

void EventCommunicationService::UnregisterAllCallbacksForListener(BaseEventCommunicator* listener)
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
