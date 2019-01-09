//
//  EntityComponentManagementService.h
//  Ninja
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef EntityComponentManagementService_h
#define EntityComponentManagementService_h

#include "IService.h"
#include "../components/IComponent.h"
#include "../TypeTraits.h"

#include <unordered_map>
#include <memory>
#include <cassert>

class EntityComponentManagementService final: public IService
{
public:
    using EntityId = long long;
    using ComponentTypeId = int;
    using EntityComponentMap = std::unordered_map<EntityId, std::unique_ptr<IComponent>>;
    
    EntityId GenerateEntity()
    {
        return mEntityCounter++;
    }

    template<class ComponentType>
    inline bool HasComponent(const EntityId eId) const
    {
        const auto componentTypeId = GetTypeHash<ComponentType>();
        return mComponentMap.at(componentTypeId).count(eId) == 1;
    }
    
    template<class ComponentType>
    inline IComponent& GetComponent(const EntityId eId)
    {
        if (!HasComponent<ComponentType>(eId))
        {
            assert(false);
        }
        
        const auto componentTypeId = GetTypeHash<ComponentType>();
        return *(mComponentMap.at(componentTypeId).contains(eId));
    }

    template<class ComponentType>
    inline const IComponent& GetComponent(const EntityId eId) const
    {
        return GetComponent<ComponentType>(eId);
    }
    
    template<class ComponentType>
    inline void AddComponent(const EntityId eId, std::unique_ptr<IComponent> component)
    {
        const auto componentTypeId = GetTypeHash<ComponentType>();
        mComponentMap[componentTypeId][eId] = std::move(component);
    }
    
    template<class ComponentType>
    inline void RemoveComponent(const EntityId eId)
    {
        const auto componentTypeId = GetTypeHash<ComponentType>();
        mComponentMap[componentTypeId].erase(eId);
    }
    
    template<class ComponentType>
    inline EntityComponentMap& GetAllComponents()
    {
        const auto componentTypeId = GetTypeHash<ComponentType>();
        return mComponentMap.at(componentTypeId);
    }
    
    template<class ComponentType>
    inline const EntityComponentMap& GetAllComponents() const
    {
        return GetAllComponents<ComponentType>();
    }
    
private:
    
    EntityId mEntityCounter = 0;
    std::unordered_map<ComponentTypeId, EntityComponentMap> mComponentMap;
    
};

#endif /* EntityComponentManagementService_h */
