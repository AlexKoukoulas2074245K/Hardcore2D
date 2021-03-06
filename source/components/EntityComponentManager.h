//
//  EntityComponentManager.h
//  Hardcore2D
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
    using ComponentTypeToImplementationMap = std::unordered_map<ComponentTypeId, std::unique_ptr<IComponent>, ComponentTypeIdHasher>;
    static const EntityId NULL_ENTITY_ID = -1;

    EntityId GenerateEntity()
    {
        return mEntityCounter++;
    }

    inline bool HasEntityEntry(const EntityId entityId)
    {
        if (entityId == NULL_ENTITY_ID)
        {
            return false;
        }

        return mEntityComponentMap.count(entityId) != 0;
    }

    inline void RemoveEntityEntry(const EntityId entityId)
    {
        if (mEntityComponentMap.count(entityId) == 0 || entityId == NULL_ENTITY_ID)
        {
            assert(false);
        }        
        mEntityComponentMap.erase(entityId);
    }

    template<class ComponentType>
    inline bool HasComponent(const EntityId entityId) const
    {
        const auto componentTypeId = GetTypeHash<ComponentType>();
        if (mEntityComponentMap.count(entityId) == 0)
        {
            assert(false);
        }
        return mEntityComponentMap.at(entityId).count(componentTypeId) == 1;
    }
    
    template<class ComponentType>
    inline ComponentType& GetComponent(const EntityId entityId)
    {
        const auto componentTypeId = GetTypeHash<ComponentType>();
        return static_cast<ComponentType&>(*(mEntityComponentMap.at(entityId).at(componentTypeId)));
    }

    template<class ComponentType>
    inline const ComponentType& GetComponent(const EntityId entityId) const
    {   
        const auto componentTypeId = GetTypeHash<ComponentType>();
        return static_cast<const ComponentType&>(*(mEntityComponentMap.at(entityId).at(componentTypeId)));
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
    
private:
    EntityComponentManager() = default;
    
    EntityId mEntityCounter = 0;
    std::unordered_map<EntityId, ComponentTypeToImplementationMap, EntityIdHasher> mEntityComponentMap;
    
};

#endif /* EntityComponentManager_h */
