//
//  NewEntityCreatedEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 01/02/2019.
//

#ifndef NewEntityCreatedEvent_h
#define NewEntityCreatedEvent_h

#include "IEvent.h"
#include "../game/GameTypeTraits.h"

class NewEntityCreatedEvent final: public IEvent
{
public:
    NewEntityCreatedEvent(const EntityNameIdEntry&);
    
    const EntityNameIdEntry& GetNewEntityNameIdEntry() const;
    
private:
    const EntityNameIdEntry mEntityNameIdEntry;
};

#endif /* NewEntityCreatedEvent_h */
