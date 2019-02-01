//
//  NewEntityCreatedEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 01/02/2019.
//

#ifndef NewEntityCreatedEvent_h
#define NewEntityCreatedEvent_h

#include "IEvent.h"
#include "../util/TypeTraits.h"

class NewEntityCreatedEvent final: public IEvent
{
public:
    NewEntityCreatedEvent(const EntityId);
    
    EntityId GetNewEntityId() const;
    
private:
    const EntityId mNewEntityId;
};

#endif /* NewEntityCreatedEvent_h */
