//
//  FlameBreathFinishedEvent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 09/03/2019.
//

#include "FlameBreathFinishedEvent.h"

FlameBreathFinishedEvent::FlameBreathFinishedEvent(const EntityId casterEntityId)
    : mCasterEntityId(casterEntityId)
{
    
}

EntityId FlameBreathFinishedEvent::GetCasterEntityId() const
{
    return mCasterEntityId;
}
