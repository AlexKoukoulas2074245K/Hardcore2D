//
//  AnnouncePlayerEntityIdEvent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 31/01/2019.
//

#include "AnnouncePlayerEntityIdEvent.h"

AnnouncePlayerEntityIdEvent::AnnouncePlayerEntityIdEvent(const EntityId playerEntityId)
    : mPlayerEntityId(playerEntityId)
{
}

EntityId AnnouncePlayerEntityIdEvent::GetPlayerEntityId() const
{
    return mPlayerEntityId;
}
