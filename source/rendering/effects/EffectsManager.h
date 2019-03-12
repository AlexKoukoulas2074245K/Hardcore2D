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
#include "../../util/TypeTraits.h"

#include <memory>

class ServiceLocator;
class EntityComponentManager;
class EventCommunicator;
class ResourceManager;
class CoreRenderingService;

class EffectsManager final : public IService
{
    friend class App;

public:
    enum class EffectType
    {
        BLOOD_SPURT_SINGLE, BLOOD_SPURT_MULTI
    };

    ~EffectsManager();

    bool VInitialize() override;
    void Update(const float dt);
    void PlayEffect(const glm::vec3& effectOrigin, const EffectType effectType);

private:
    static const float MAX_BLUR_UPON_DEATH;
    static const float BLUR_SPEED_UPON_DEATH;

    EffectsManager(const ServiceLocator&);
    void CreateBloodSpurtSingleEffect(const glm::vec3& effectOrigin);
    void CreateBloodSpurtMultiEffect(const glm::vec3& effectOrigin);
    void RegisterEventCallbacks();
    void CreateDeathQuoteEntity();
    void CreateRespawnQuoteEntity();
    
    const ServiceLocator& mServiceLocator;    
    EntityComponentManager* mEntityComponentManager;
    ResourceManager* mResourceManager;
    CoreRenderingService* mCoreRenderingService;

    std::unique_ptr<EventCommunicator> mEventCommunicator;
    float mSceneBlurIntensity;
    bool mIsBlurIntensifying;
    EntityId mDeathQuoteEntityId;
    EntityId mRespawnQuoteEntityId;
};

#endif /* EffectsManager_h */
