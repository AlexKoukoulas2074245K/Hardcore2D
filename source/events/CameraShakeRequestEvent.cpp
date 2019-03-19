//
//  CameraShakeRequestEvent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/03/2019.
//

#include "CameraShakeRequestEvent.h"

CameraShakeRequestEvent::CameraShakeRequestEvent(const float shakeRadius)
    : mShakeRadius(shakeRadius)
{
    
}

float CameraShakeRequestEvent::GetShakeRadius() const
{
    return mShakeRadius;
}

