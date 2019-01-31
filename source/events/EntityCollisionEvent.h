//
//  EntityCollisionEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#ifndef EntityCollisionEvent_h
#define EntityCollisionEvent_h

#include "IEvent.h"
#include "../util/TypeTraits.h"

#include <utility>

class EntityCollisionEvent final: public IEvent
{
public:
    EntityCollisionEvent(const std::pair<EntityId, EntityId>& collidedEntityIds);
    
    const std::pair<EntityId, EntityId>& GetCollidedEntityIds() const;
    
private:
    const std::pair<EntityId, EntityId> mCollidedEntityIds;
};


#endif /* EntityCollisionEvent_h */
