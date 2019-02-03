//
//  App.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef App_h
#define App_h

#include <memory>
#include <vector>

class AIService;
class Camera;
class ServiceLocator;
class DamageSystem;
class EntityComponentManager;
class EventCommunicator;
class EventCommunicationService;
class CoreRenderingService;
class AnimationService;
class ResourceManager;
class PhysicsSystem;
class InputHandler;
class IInputActionConsumer;
class Level;

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
    std::unique_ptr<EventCommunicationService> mEventCommunicationService;    
    std::unique_ptr<EntityComponentManager> mEntityComponentManager;
    std::unique_ptr<AIService> mAIService;
    std::unique_ptr<CoreRenderingService> mCoreRenderingService;
    std::unique_ptr<AnimationService> mAnimationService;
    std::unique_ptr<ResourceManager> mResourceManager;
    std::unique_ptr<InputHandler> mInputHandler;
    std::unique_ptr<PhysicsSystem> mPhysicsSystem;
    std::unique_ptr<DamageSystem> mDamageSystem;
    
    std::vector<std::unique_ptr<IInputActionConsumer>> mInputActionConsumers;
    
    std::unique_ptr<EventCommunicator> mEventCommunicator;
    std::unique_ptr<Camera> mCamera;
    std::unique_ptr<Level> mLevel;
};

#endif /* App_h */
