//
//  HealthComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 03/02/2019.
//

#ifndef HealthComponent_h
#define HealthComponent_h

#include "IComponent.h"

class HealthComponent final: public IComponent
{
public:
    HealthComponent(const float health, const float invincibilityDuration = DEFAULT_INVINCIBILITY_DURATION);
    
    bool IsTemporarilyInvincible() const;
    float GetInvincibilityTimer() const;
    float GetInvincibilityDuration() const;
    float GetHealth() const;
    
    void SetTemporarilyInvincible(const bool isTemporarilyInvincible);
    void SetInvincibilityTimer(const float invincibilityTimer);
    void ReduceHealthBy(const float health);
    
private:
    static const float DEFAULT_INVINCIBILITY_DURATION;
    
    const float mInvincibilityDuration;
    bool mIsTemporarilyInvincible;
    float mInvincibilityTimer;
    float mHealth;
};

#endif /* HealthComponent_h */
