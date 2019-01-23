//
//  CoreRenderingService.h
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef CoreRenderingService_h
#define CoreRenderingService_h

#include "../IService.h"
#include "../util/TypeTraits.h"

#include <functional>
#include <vector>
#include <string>
#include <map>
#include <memory>

struct SDL_Window;
struct SDL_Context;
union SDL_Event;

class EntityComponentManager;
class ResourceManager;
class ServiceLocator;
class Shader;
class EventCommunicator;


class CoreRenderingService final: public IService
{
    friend class App;
public:
    
    ~CoreRenderingService();
    
    bool InitializeEngine();
    void GameLoop(std::function<void(const float)> appUpdateMethod);
    void RenderEntities(const std::vector<EntityId>& entityIds);
    
    float GetRenderableWidth() const;
    float GetRenderableHeight() const;
    
private:
    using SDL_GLContext = void*;
  
    
    CoreRenderingService(const ServiceLocator&);
    
    bool InitializeContext();
    void InitializeRenderingPrimitive();
    void CompileAllShaders();
    void RegisterEventCallbacks();

    void RenderEntity(const EntityId);
	void PrepareSpecificShaderUniformsForEntityRendering(const EntityId);

    const ServiceLocator& mServiceLocator;
    EntityComponentManager* mEntityComponentManager;
    ResourceManager* mResourceManager;

    std::unique_ptr<EventCommunicator> mEventCommunicator;
    SDL_Window* mSdlWindow;
    SDL_GLContext mSdlGLContext;
    
    bool mDebugHitboxDisplay;
    bool mRunning;
    
    float mRenderableAreaWidth;
    float mRenderableAreaHeight;
	float mAspectRatio;

    GLuint mVAO;
    GLuint mVBO;

    std::map<std::string, std::unique_ptr<Shader>> mShaders;
    std::string mCurrentShaderUsed;
};


#endif /* CoreRenderingService_h */
