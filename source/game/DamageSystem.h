//
//  DamageSystem.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 02/02/2019.
//

#ifndef DamageSystem_h
#define DamageSystem_h

#include "../IService.h"

#include <memory>

class ServiceLocator;
class EntityComponentManager;
class EventCommunicator;

class DamageSystem final: public IService
{
    friend class App;
public:
    bool Initialize();
    
private:
    DamageSystem(const ServiceLocator&);
    
    const ServiceLocator& mServiceLocator;
    EntityComponentManager* mEntityComponentManager;
    std::unique_ptr<EventCommunicator> mEventCommunicator;
};

#endif /* DamageSystem_h */
