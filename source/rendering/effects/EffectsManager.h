//
//  EffectsManager.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/02/2019.
//

#ifndef EffectsManager_h
#define EffectsManager_h

#include "../../IService.h"
#include "../../util/MathUtils.h"

#include <memory>

class ServiceLocator;
class EntityComponentManager;
class EventCommunicator;
class ResourceManager;

class EffectsManager final : public IService
{
    friend class App;

public:
    enum class EffectType
    {
        BLOOD_SPURT
    };

    ~EffectsManager();

    bool VInitialize() override;    
    void PlayEffect(const glm::vec3& effectOrigin, const EffectType effectType);

private:
    EffectsManager(const ServiceLocator&);
    void CreateBloodSpurtEffect(const glm::vec3& effectOrigin);

    const ServiceLocator& mServiceLocator;    
    EntityComponentManager* mEntityComponentManager;
    ResourceManager* mResourceManager;

    std::unique_ptr<EventCommunicator> mEventCommunicator;
};

#endif /* EffectsManager_h */
