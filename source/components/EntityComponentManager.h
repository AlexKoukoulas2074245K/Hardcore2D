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
    friend class App;
public:
    using ComponentTypeToImplementationMap = std::unordered_map<ComponentTypeId, std::unique_ptr<IComponent>>;
    
    EntityId GenerateEntity()
    {
        return mEntityCounter++;
    }

    template<class ComponentType>
    inline bool HasComponent(const EntityId entityId) const
    {
        const auto componentTypeId = GetTypeHash<ComponentType>();
        return mEntityComponentMap.at(entityId).count(componentTypeId) == 1;
    }
    
    template<class ComponentType>
    inline ComponentType& GetComponent(const EntityId entityId)
    {
        if (!HasComponent<ComponentType>(entityId))
        {
            assert(false);
        }
        
        const auto componentTypeId = GetTypeHash<ComponentType>();
        return static_cast<ComponentType&>(*(mEntityComponentMap.at(entityId).at(componentTypeId)));
    }

    template<class ComponentType>
    inline const ComponentType& GetComponent(const EntityId entityId) const
    {
        return GetComponent<ComponentType>(entityId);
    }
    
    template<class ComponentType>
    inline void AddComponent(const EntityId entityId, std::unique_ptr<IComponent> component)
    {
        const auto componentTypeId = GetTypeHash<ComponentType>();
        mEntityComponentMap[entityId][componentTypeId] = std::move(component);
    }
    
    template<class ComponentType>
    inline void RemoveComponent(const EntityId entityId)
    {
        const auto componentTypeId = GetTypeHash<ComponentType>();
        mEntityComponentMap[entityId].erase(componentTypeId);
    }
    
    template<class ComponentType>
    inline std::unordered_map<EntityId, IComponent*> GetAllComponents()
    {
        const auto componentTypeId = GetTypeHash<ComponentType>();
        std::unordered_map<EntityId, IComponent*> result;
        
        for (const auto entry: mEntityComponentMap)
        {
            if (entry.second.count(componentTypeId) != 0)
            {
                result[entry.first] = entry.second.at(componentTypeId).get();
            }
        }
        
        return result;
    }
    
    inline const ComponentTypeToImplementationMap& GetAllEntityComponents(const EntityId entityId)
    {
        return mEntityComponentMap.at(entityId);
    }
    
    
private:
    EntityComponentManager() = default;
    
    EntityId mEntityCounter = 0;
    std::unordered_map<EntityId, ComponentTypeToImplementationMap> mEntityComponentMap;
    
};

#endif /* EntityComponentManager_h */
