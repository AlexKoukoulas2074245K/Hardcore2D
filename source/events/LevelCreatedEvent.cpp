//
//  LevelCreatedEvent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 01/03/2019.
//

#include "LevelCreatedEvent.h"

LevelCreatedEvent::LevelCreatedEvent(const float levelWidth, const float levelHeight)
    : mLevelWidth(levelWidth)
    , mLevelHeight(levelHeight)
{
    
}

float LevelCreatedEvent::GetLevelWidth() const
{
    return mLevelWidth;
}

float LevelCreatedEvent::GetLevelHeight() const
{
    return mLevelHeight;
}
