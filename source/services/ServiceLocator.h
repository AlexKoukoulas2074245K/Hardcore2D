//
//  ServiceLocator.h
//  Ninja
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef ServiceLocator_h
#define ServiceLocator_h

#include "../TypeTraits.h"

#include <unordered_map>
#include <memory>
#include <cassert>

class IService;
class ServiceLocator
{
public:
    ServiceLocator() = default;
    ServiceLocator(const ServiceLocator&) = delete;
    ServiceLocator& operator = (const ServiceLocator&) = delete;
    
    template <class ServiceType>
    inline ServiceType& ResolveService() const
    {
        
        const auto serviceTypeKey = GetTypeHash<ServiceType>();
        assert(mServices.count(serviceTypeKey) != 0);
        return static_cast<ServiceType&>(*(mServices.at(serviceTypeKey)));
    }
    
    template <class ServiceType>
    inline void RegisterService(IService* service)
    {
        const auto serviceTypeKey = GetTypeHash<ServiceType>();
        assert(mServices.count(serviceTypeKey) == 0);
        mServices[serviceTypeKey] = service;
    }
    
private:
    std::unordered_map<size_t, IService*> mServices;
};

#endif /* ServiceLocator_h */
