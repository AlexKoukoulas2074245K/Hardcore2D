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

#include <unordered_set>

class DamageComponent final: public IComponent
{
public:
    DamageComponent(const EntityId ownerEntityId, const float damage, const bool canDamageSameEntityMultipleTimes);
    
    EntityId GetOwnerEntityId() const;
    float GetDamage() const;
    bool CanDamageSameEntityMultipleTimes() const;
    bool IsEntityWhitelisted(const EntityId) const;

    void AddEntityToWhitelistedDamagedEntities(const EntityId);

    
private:
    const EntityId mOwnerEntityId;
    const float mDamage;
    const bool mCanDamageSameEntityMultipleTimes;
    std::unordered_set<EntityId> mWhitelistedDamagedEntities;
};

#endif /* DamageComponent_h */
