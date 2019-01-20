//
//  App.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef App_h
#define App_h

#include "util/TypeTraits.h"

#include <memory>
#include <vector>

class ServiceLocator;
class EntityComponentManager;
class EventCommunicationService;
class CoreRenderingService;
class ResourceManager;
class PhysicsSystem;
class InputHandler;
class PlayerInputController;

union SDL_Event;

class App final
{
public:
    App();
    ~App();
    
    bool Initialize();
    void Run();
    
private:
    void Update(const float dt);
	void HandleInput();

    std::unique_ptr<ServiceLocator> mServiceLocator;
    std::unique_ptr<EntityComponentManager> mEntityComponentManager;
    std::unique_ptr<EventCommunicationService> mEventCommunicationService;
    std::unique_ptr<CoreRenderingService> mCoreRenderingService;
    std::unique_ptr<ResourceManager> mResourceManager;
    std::unique_ptr<InputHandler> mInputHandler;
    std::unique_ptr<PhysicsSystem> mPhysicsSystem;
    std::unique_ptr<PlayerInputController> mPlayerInputController;
    
    std::vector<EntityId> mActiveEntityIds;
};

#endif /* App_h */
