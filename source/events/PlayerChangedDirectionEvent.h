//
//  PlayerChangedDirectionEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 26/01/2019.
//

#ifndef PlayerChangedDirectionEvent_h
#define PlayerChangedDirectionEvent_h

#include "IEvent.h"
#include "../GameConstants.h"

class PlayerChangedDirectionEvent final: public IEvent
{
public:
    PlayerChangedDirectionEvent(const FacingDirection newFacingDirection);
    
    FacingDirection GetNewFacingDirection() const;
    
private:
    const FacingDirection mNewFacingDirection;
    
};


#endif /* PlayerChangedDirectionEvent_h */
