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

class BasicNinjaEnemyAIComponent final: public IAIComponent
{
public:
    BasicNinjaEnemyAIComponent(const ServiceLocator&, const EntityId thisEntityId);
    ~BasicNinjaEnemyAIComponent();

    void VUpdate(const float dt) override;
    
private:
    enum class State
    {
        INITIALIZE, PATROLLING, LEAPING_TO_TARGET, FOCUSING, CHARGING_PLAYER, DEAD 
    };

    static const float PLAYER_DETECTION_DISTANCE;
    static const float PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION;
    static const float PURSUING_MAX_DISTANCE_HORIZONTALLY;
    static const float FOCUSING_TIMER;
    static const float CHARGING_TIMER;
    
    void OnAnnouncePlayerEntityId(const IEvent&);
    void OnEntityCollisionEvent(const IEvent&);
    void OnLeapingComplete(const PhysicsComponent&);
    
    EntityComponentManager& mEntityComponentManager;
    const EntityId mThisEntityId;        
    EntityId mTargetEntityId;
    State mState;
    bool mMovingRight;
    glm::vec3 mInitPosition;
    float mTimer;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
};

#endif /* BasicNinjaEnemyAIComponent_h */
