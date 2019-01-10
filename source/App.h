//
//  App.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef App_h
#define App_h

#include <memory>

class ServiceLocator;
class EntityComponentManager;
class EventCommunicationService;
class CoreEngineService;

class App final
{
public:
    App();
    ~App();
    
private:
    void CreateAndRegisterServices();
    void Run();
    void Update(const float dt);
    
    std::unique_ptr<ServiceLocator> mServiceLocator;
    std::unique_ptr<EntityComponentManager> mEntityComponentManager;
    std::unique_ptr<EventCommunicationService> mEventCommunicationService;
    std::unique_ptr<CoreEngineService> mCoreEngineService;
};

#endif /* App_h */
