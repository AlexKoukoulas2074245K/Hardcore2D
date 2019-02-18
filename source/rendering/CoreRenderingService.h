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
#include "../util/StringId.h"
#include "../game/GameTypeTraits.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <functional>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

struct SDL_Window;

class EntityComponentManager;
class ResourceManager;
class ServiceLocator;
class Shader;
class EventCommunicator;
class Camera;

class CoreRenderingService final: public IService
{
    friend class App;
public:
    
    ~CoreRenderingService();
    
    bool VInitialize() override;
    
    void AttachCamera(const Camera* camera);
    void GameLoop(std::function<void(const float)> appUpdateMethod);
    void RenderEntities(const std::vector<EntityNameIdEntry>& entityIds);
    void UpdateCamera(const EntityId focusedEntity, const float dt);
    
    const glm::vec2& GetRenderableDimensions() const;
    
private:
    using SDL_GLContext = void*;
  
    
    CoreRenderingService(const ServiceLocator&);
    
    bool InitializeContext();
    void InitializeRenderingPrimitive();
    void CompileAllShaders();
    void RegisterEventCallbacks();

    void RenderEntityInternal(const EntityId);
    void PreparePostProcessingPass();
        
    const ServiceLocator& mServiceLocator;
    EntityComponentManager* mEntityComponentManager;
    ResourceManager* mResourceManager;

    std::unique_ptr<EventCommunicator> mEventCommunicator;
    SDL_Window* mSdlWindow;
    SDL_GLContext mSdlGLContext;
    
    bool mDebugHitboxDisplay;
    bool mRunning;
    
    glm::vec2 mRenderableDimensions;
    glm::mat4 mProjectionMatrix;
    
    float mSwirlAngle;
    
    const Camera* mAttachedCamera;
    
    GLuint mVAO, mVBO, mFrameBufferId, mScreenRenderingTexture;

    std::unordered_map<StringId, std::unique_ptr<Shader>, StringIdHasher> mShaders;
    StringId mCurrentShader;
};


#endif /* CoreRenderingService_h */
