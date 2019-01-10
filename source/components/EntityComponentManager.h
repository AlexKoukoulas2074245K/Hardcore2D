//
//  EntityComponentManager.h
//  Ninja
//
//  Created by Alex Koukoulas on 07/01/2019.
//

#ifndef EntityComponentManager_h
#define EntityComponentManager_h

#include "../IService.h"
#include "../components/IComponent.h"
#include "../util/TypeTraits.h"

#include <unordered_map>
#include <memory>
#include <cassert>

class EntityComponentManager final: public IService
{
public:
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

#endif /* EntityComponentManager_h */
