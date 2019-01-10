//
//  EventCommunicationService.h
//  Ninja
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef EventCommunicationService_h
#define EventCommunicationService_h

#include "../IService.h"
#include "IEvent.h"
#include "../util/TypeTraits.h"

#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

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
    
    void DispatchEvent(BaseEventCommunicator* dispatcher, std::unique_ptr<IEvent> evt);
    
    void UnregisterAllCallbacksForListener(BaseEventCommunicator* listener);
    
private:
    std::unordered_map<size_t, std::vector<std::pair<BaseEventCommunicator*, EventCallback>>> mEventCallbackRegistry;
    std::vector<std::pair<std::string, BaseEventCommunicator*>> mEventDispatchingHistory;
};

#endif /* EventCommunicationService_h */
