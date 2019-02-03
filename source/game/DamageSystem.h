//
//  DamageSystem.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 02/02/2019.
//

#ifndef DamageSystem_h
#define DamageSystem_h

#include "../IService.h"
#include "../game/GameConstants.h"

#include <memory>
#include <vector>

class ServiceLocator;
class HealthComponent;
class EntityComponentManager;
class EventCommunicator;
class IEvent;

class DamageSystem final: public IService
{
    friend class App;
public:
    bool VInitialize() override;
    
    void Update(const std::vector<EntityNameIdEntry>&, const float dt);
    
private:
    DamageSystem(const ServiceLocator&);
    
    void OnEntityCollisionEvent(const IEvent&);
    
    const ServiceLocator& mServiceLocator;
    EntityComponentManager* mEntityComponentManager;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
};

#endif /* DamageSystem_h */
