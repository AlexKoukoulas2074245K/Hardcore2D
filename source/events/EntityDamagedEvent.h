//
//  EntityDamagedEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#ifndef EntityDamagedEvent_h
#define EntityDamagedEvent_h

#include "IEvent.h"
#include "../util/TypeTraits.h"

class EntityDamagedEvent final: public IEvent
{
public:
    EntityDamagedEvent(const EntityId damagedEntityId, const float damageDone, const float healthRemaining);
    
    EntityId GetDamagedEntityId() const;
    float GetDamageDone() const;
    float GetHealthRemaining() const;

private:
    const EntityId mDamagedEntityId;
    const float mDamageDone;
    const float mHealthRemaining;
};

#endif /* EntityDamagedEvent_h */
