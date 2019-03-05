//
//  BasicNinjaEnemyAIComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 29/01/2019.
//

#ifndef BasicNinjaEnemyAIComponent_h
#define BasicNinjaEnemyAIComponent_h

#include "IAIComponent.h"
#include "../util/TypeTraits.h"

#include <memory>
#include <glm/vec3.hpp>

class EntityComponentManager;
class ServiceLocator;
class EventCommunicator;
class IEvent;
class PhysicsComponent;
class EffectsManager;

class BasicNinjaEnemyAIComponent final: public IAIComponent
{
public:
    BasicNinjaEnemyAIComponent(const ServiceLocator&, const EntityId thisEntityId);
    ~BasicNinjaEnemyAIComponent();

    void VUpdate(const float dt) override;
    
private:
    enum class State
    {
        INITIALIZE, PATROLLING, PURSUING, DEAD 
    };

    static const float PLAYER_DETECTION_DISTANCE;
    static const float PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION;
    static const float PURSUING_MELEE_ATTACK_DISTANCE;
    static const float MELEE_ATTACK_COOLDOWN;

    void OnAnnouncePlayerEntityId(const IEvent&);
    void OnEntityCollisionEvent(const IEvent&);
    void OnEntityDamagedEvent(const IEvent&);
    void OnLeapingComplete();
    
    const ServiceLocator& mServiceLocator;
    EntityComponentManager& mEntityComponentManager;
    EffectsManager& mEffectsManager;
    std::unique_ptr<EventCommunicator> mEventCommunicator;

    const EntityId mThisEntityId;        
    EntityId mTargetEntityId;
    State mState;
    bool mMovingRight;
    glm::vec3 mInitPosition;
    float mDamagePushback;
    float mTimer;    
};

#endif /* BasicNinjaEnemyAIComponent_h */
