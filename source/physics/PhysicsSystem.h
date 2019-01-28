//
//  PhysicsSystem.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/01/2019.
//

#ifndef PhysicsSystem_h
#define PhysicsSystem_h

#include "../IService.h"
#include "../game/GameConstants.h"

#include <vector>
#include <utility>

class ServiceLocator;
class EntityComponentManager;

class PhysicsSystem final: public IService
{
    friend class App;
public:
    void Initialize();
    void UpdateEntities(const std::vector<EntityNameIdEntry>& activeEntities, const float dt);
    
private:
    PhysicsSystem(const ServiceLocator&);
    
    enum class Axis
    {
        X_AXIS, Y_AXIS
    };

    EntityId CheckAndGetCollidedEntity(const EntityId referenceEntityId, const std::vector<EntityNameIdEntry>& allConsideredEntityIds);
    void PushEntityOutsideOtherEntityInAxis(const EntityId referenceEntityId, const EntityId collidedWithEntityId, const Axis, const float dt);

    const ServiceLocator& mServiceLocator;
    EntityComponentManager* mEntityComponentManager;
};

#endif /* PhysicsSystem_h */
