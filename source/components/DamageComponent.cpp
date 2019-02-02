//
//  DamageComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 02/02/2019.
//

#include "DamageComponent.h"

DamageComponent::DamageComponent(const float damage)
    : mDamage(damage)
{
    
}

float DamageComponent::GetDamage() const
{
    return mDamage;
}
