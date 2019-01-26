//
//  PlayerChangedDirectionEvent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 26/01/2019.
//

#include "PlayerChangedDirectionEvent.h"

PlayerChangedDirectionEvent::PlayerChangedDirectionEvent(const FacingDirection newFacingDirection)
    : mNewFacingDirection(newFacingDirection)
{
}

FacingDirection PlayerChangedDirectionEvent::GetNewFacingDirection() const
{
    return mNewFacingDirection;
}
