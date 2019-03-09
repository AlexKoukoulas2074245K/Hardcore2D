//
//  FlameBreathFinishedEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 09/03/2019.
//

#ifndef FlameBreathFinishedEvent_h
#define FlameBreathFinishedEvent_h

#include "IEvent.h"
#include "../util/TypeTraits.h"

class FlameBreathFinishedEvent final: public IEvent
{
public:
    FlameBreathFinishedEvent(const EntityId casterEntityId);
    
    EntityId GetCasterEntityId() const;
    
private:
    const EntityId mCasterEntityId;
};

#endif /* FlameBreathFinishedEvent_h */
