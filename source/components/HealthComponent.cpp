//
//  HealthComponent.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#include "HealthComponent.h"

const float HealthComponent::DEFAULT_INVINCIBILITY_DURATION = 0.5f;

HealthComponent::HealthComponent(const float health, const float invincibilityDuration /* DEFAULT_INVINCIBILITY_DURATION */ )
    : mInvincibilityDuration(invincibilityDuration)
    , mIsTemporarilyInvincible(false)
    , mInvincibilityTimer(0.0f)
    , mHealth(health)
{
    
}

bool HealthComponent::IsTemporarilyInvincible() const
{
    return mIsTemporarilyInvincible;
}

float HealthComponent::GetInvincibilityTimer() const
{
    return mInvincibilityTimer;
}

float HealthComponent::GetInvincibilityDuration() const
{
    return mInvincibilityDuration;
}

float HealthComponent::GetHealth() const
{
    return mHealth;
}

void HealthComponent::SetTemporarilyInvincible(const bool isTemporarilyInvincible)
{
    mIsTemporarilyInvincible = isTemporarilyInvincible;
}

void HealthComponent::SetInvincibilityTimer(const float invincibilityTimer)
{
    mInvincibilityTimer = invincibilityTimer;
}

void HealthComponent::ReduceHealthBy(const float health)
{
    mHealth -= health;
}
