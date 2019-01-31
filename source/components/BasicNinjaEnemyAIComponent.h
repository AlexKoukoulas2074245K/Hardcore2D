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

class BasicNinjaEnemyAIComponent final: public IAIComponent
{
public:
    BasicNinjaEnemyAIComponent(const ServiceLocator&, const EntityId thisEntityId);
    ~BasicNinjaEnemyAIComponent();

    void VUpdate(const float dt) override;
    std::string VSerializeToString() const override;
    bool VInitializeFromString(const std::string&) override;
    
private:
    enum class State
    {
        INITIALIZE, PATROLLING, CHARGING, DEAD 
    };

    static const float PLAYER_DETECTION_DISTANCE;
    static const float PATROLLING_MAX_DISTANCE_FROM_INIT_POSITION;

    const ServiceLocator& mServiceLocator;
    const EntityComponentManager& mEntityComponentManager;
    const EntityId mThisEntityId;        
    EntityId mTargetEntityId;
    State mState;
    bool mMovingRight;
    glm::vec3 mInitPosition;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
};

#endif /* BasicNinjaEnemyAIComponent_h */
