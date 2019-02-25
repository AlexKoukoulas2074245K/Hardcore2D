//
//  HorizontalMovingPlatformAIComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 23/02/2019.
//

#ifndef HorizontalMovingPlatformAIComponent_h
#define HorizontalMovingPlatformAIComponent_h

#include "IAIComponent.h"
#include "../util/TypeTraits.h"
#include "../util/MathUtils.h"

class ServiceLocator;
class EntityComponentManager;

class HorizontalMovingPlatformAIComponent final: public IAIComponent
{
public:
    HorizontalMovingPlatformAIComponent(const ServiceLocator&, const EntityId entityId);
    
    void VUpdate(const float dt) override;
    
private:
    static const float MOVEMENT_DURATION_PER_DIRECTION;
    
    EntityComponentManager& mEntityComponentManager;
    const EntityId mEntityId;    
    bool mMovingRight;
    float mTimer;
    
};

#endif /* HorizontalMovingPlatformAIComponent_h */
