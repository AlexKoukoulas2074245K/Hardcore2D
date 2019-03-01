//
//  LevelCreatedEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 01/03/2019.
//

#ifndef LevelCreatedEvent_h
#define LevelCreatedEvent_h

#include "IEvent.h"

class LevelCreatedEvent final: public IEvent
{
public:
    LevelCreatedEvent(const float levelWidth, const float levelHeight);
    
    float GetLevelWidth() const;
    float GetLevelHeight() const;
    
private:
    const float mLevelWidth;
    const float mLevelHeight;
};


#endif /* LevelCreatedEvent_h */
