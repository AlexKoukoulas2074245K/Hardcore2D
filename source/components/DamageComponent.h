//
//  DamageComponent.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 02/02/2019.
//

#ifndef DamageComponent_h
#define DamageComponent_h

#include "IComponent.h"
#include "../util/TypeTraits.h"

#include <vector>

class DamageComponent final: public IComponent
{
public:
    DamageComponent(const EntityId ownerEntityId, const float damage);
    
    EntityId GetOwnerEntityId() const;
    float GetDamage() const;
    
private:
    const EntityId mOwnerEntityId;
    const float mDamage;
};

#endif /* DamageComponent_h */
