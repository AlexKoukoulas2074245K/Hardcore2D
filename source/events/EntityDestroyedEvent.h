//
//  EntityDestroyedEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 01/02/2019.
//

#ifndef EntityDestroyedEvent_h
#define EntityDestroyedEvent_h

#include "IEvent.h"
#include "../util/TypeTraits.h"

class EntityDestroyedEvent final : public IEvent
{
public:
    EntityDestroyedEvent(const EntityId);

    EntityId GetDestroyedEntityId() const;

private:
    const EntityId mEntityId;
};

#endif /* EntityDestroyedEvent_h */
