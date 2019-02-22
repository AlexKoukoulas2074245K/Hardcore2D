//
//  PlayerHealthbarUIElement.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/02/2019.
//

#ifndef PlayerHealthbarUIElement_h
#define PlayerHealthbarUIElement_h

#include "IUIElement.h"

#include <memory>

class CoreRenderingService;
class ServiceLocator;
class EntityComponentManager;
class EventCommunicator;
class ResourceManager;

class PlayerHealthbarUIElement final: public IUIElement
{
public:
    PlayerHealthbarUIElement(const ServiceLocator&);
    ~PlayerHealthbarUIElement();
    
    void VUpdate(const float dt) override;
    const std::vector<EntityId>& VGetEntityIds() const override;
    
private:
    static const float HEALTH_DEPLETION_ANIMATION_DURATION;

    void InitializeHealthbarEntities();
    void RegisterEventCallbacks();

    const CoreRenderingService& mCoreRenderingService;
    EntityComponentManager& mEntityComponentManager;
    ResourceManager& mResourceManager;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    
    EntityId mPlayerId;
    std::vector<EntityId> mEntityIds;

    float mStartingHealth;    
    float mCurrentHealth;
    float mAnimationStartingHealth;
    float mAnimationTimer;
};

#endif /* PlayerHealthbarUIElement_h */
