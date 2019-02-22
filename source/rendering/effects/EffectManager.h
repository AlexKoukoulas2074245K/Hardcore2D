//
//  EffectManager.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/02/2019.
//

#ifndef EffectManager_h
#define EffectManager_h

#include "../../IService.h"
#include "../../util/TypeTraits.h"

#include <memory>

class ServiceLocator;
class EntityComponentManager;
class EventCommunicator;
class ResourceManager;

class EffectManager final : public IService
{
    friend class App;

public:
    enum class EffectType
    {
        BLOOD_SPURT
    };

    ~EffectManager();

    bool VInitialize() override;    
    void PlayEffect(const EntityId originEntityId, const EffectType effectType);

private:
    EffectManager(const ServiceLocator&);
    void CreateBloodSpurtEffect(const EntityId originEntityId);

    const ServiceLocator& mServiceLocator;    
    EntityComponentManager* mEntityComponentManager;
    ResourceManager* mResourceManager;

    std::unique_ptr<EventCommunicator> mEventCommunicator;
};

#endif /* EffectManager_h */
