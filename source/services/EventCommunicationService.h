//
//  EventCommunicationService.h
//  Ninja
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef EventCommunicationService_h
#define EventCommunicationService_h

#include "IService.h"
#include "../events/IEvent.h"
#include "../TypeTraits.h"

#include <memory>
#include <unordered_map>
#include <vector>

class IEvent;
class BaseEventCommunicator;
class EventCommunicationService final: public IService
{
public:
    using EventCallback = std::function<void(const IEvent&)>;
    
    template <class EventType>
    inline void RegisterEventCallback(BaseEventCommunicator* listener, EventCallback eventCallback)
    {
        const auto eventTypeHash = GetTypeHash<EventType>();
        mEventCallbackRegistry[eventTypeHash].push_back(std::make_pair(listener, eventCallback));
    }
    
    inline void DispatchEvent(BaseEventCommunicator* dispatcher, std::unique_ptr<IEvent> evt)
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
    
    inline void UnregisterAllCallbacksForListener(BaseEventCommunicator* listener)
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
    
    void Reset();
    
private:
    std::unordered_map<size_t, std::vector<std::pair<BaseEventCommunicator*, EventCallback>>> mEventCallbackRegistry;
    std::vector<std::pair<std::string, BaseEventCommunicator*>> mEventDispatchingHistory;
};

#endif /* EventCommunicationService_h */
