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
class ResourceManager;

class App final
{
public:
    App();
    ~App();
    
    bool Initialize();
    void Run();
    
private:
    void Update(const float dt);
    
    std::unique_ptr<ServiceLocator> mServiceLocator;
    std::unique_ptr<EntityComponentManager> mEntityComponentManager;
    std::unique_ptr<EventCommunicationService> mEventCommunicationService;
    std::unique_ptr<CoreEngineService> mCoreEngineService;
    std::unique_ptr<ResourceManager> mResourceManager;
};

#endif /* App_h */
