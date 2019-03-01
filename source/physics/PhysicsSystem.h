//
//  PhysicsSystem.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 19/01/2019.
//

#ifndef PhysicsSystem_h
#define PhysicsSystem_h

#include "../IService.h"
#include "../game/GameTypeTraits.h"

#include <vector>
#include <list>
#include <utility>
#include <memory>

class ServiceLocator;
class EntityComponentManager;
class EventCommunicator;
class ISceneGraph;

class PhysicsSystem final: public IService
{
    friend class App;
public:
    ~PhysicsSystem();
    bool VInitialize() override;
    void UpdateEntities(const std::vector<EntityNameIdEntry>& activeEntities, const float dt);
    
private:
    enum class Axis
    {
        X_AXIS, Y_AXIS
    };
    
    PhysicsSystem(const ServiceLocator&);
    
    void RegisterEventCallbacks();
    std::list<EntityId> CheckAndGetCollidedEntities(const EntityId referenceEntityId, const std::list<EntityId>& allConsideredEntityIds);
    void PushEntityOutsideOtherEntityInAxis(const EntityId referenceEntityId, const EntityId collidedWithEntityId, const Axis, const float dt);

    const ServiceLocator& mServiceLocator;
    EntityComponentManager* mEntityComponentManager;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    std::unique_ptr<ISceneGraph> mSceneGraph;
};

#endif /* PhysicsSystem_h */
