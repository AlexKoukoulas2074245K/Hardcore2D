//
//  AnnouncePlayerEntityIdEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#ifndef AnnouncePlayerEntityIdEvent_h
#define AnnouncePlayerEntityIdEvent_h

#include "IEvent.h"
#include "../util/TypeTraits.h"

class AnnouncePlayerEntityIdEvent final: public IEvent
{
public:
    AnnouncePlayerEntityIdEvent(const EntityId playerEntityId);

    EntityId GetPlayerEntityId() const;
    
private:
    const EntityId mPlayerEntityId;
};


#endif /* AnnouncePlayerEntityIdEvent_h */
