//
//  VerticalMovingPlatformAIComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 23/02/2019.
//

#ifndef VerticalMovingPlatformAIComponent_h
#define VerticalMovingPlatformAIComponent_h

#include "IAIComponent.h"
#include "../util/TypeTraits.h"
#include "../util/MathUtils.h"

class ServiceLocator;
class EntityComponentManager;

class VerticalMovingPlatformAIComponent final: public IAIComponent
{
public:
    VerticalMovingPlatformAIComponent(const ServiceLocator&, const EntityId entityId);
    
    void VUpdate(const float dt) override;
    
private:
    static const float MAX_DISTANCE_FROM_INIT_POSITION;
    
    EntityComponentManager& mEntityComponentManager;
    const EntityId mEntityId;
    const glm::vec3 mInitPosition;
    bool mMovingDownwards;
    
};

#endif /* VerticalMovingPlatformAIComponent_h */
