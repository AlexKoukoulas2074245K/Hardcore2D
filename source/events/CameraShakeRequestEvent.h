//
//  CameraShakeRequestEvent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/03/2019.
//

#ifndef CameraShakeRequestEvent_h
#define CameraShakeRequestEvent_h

#include "IEvent.h"

class CameraShakeRequestEvent final: public IEvent
{
public:
    CameraShakeRequestEvent(const float shakeRadius);
    
    float GetShakeRadius() const;
    
private:
    const float mShakeRadius;
};

#endif /* CameraShakeRequestEvent_h */
