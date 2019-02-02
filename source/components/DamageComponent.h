//
//  DamageComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 02/02/2019.
//

#ifndef DamageComponent_h
#define DamageComponent_h

#include "IComponent.h"

class DamageComponent final: public IComponent
{
public:
    DamageComponent(const float damage);
    
    float GetDamage() const;
    
private:
    const float mDamage;
};

#endif /* DamageComponent_h */
