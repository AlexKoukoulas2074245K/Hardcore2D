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

class ServiceLocator;
class Shader;

class CoreRenderingService final: public IService
{
    friend class App;
public:
    
    ~CoreRenderingService();
    
    bool InitializeEngine();
    void GameLoop(std::function<void(const float)> clientUpdateMethod);
    void RenderEntity(const EntityId);
    
    int GetRenderableWidth() const;
    int GetRenderableHeight() const;
    
private:
    using SDL_GLContext = void*;
  
    
    CoreRenderingService(const ServiceLocator&);
    
    bool InitializeContext();
    void InitializeRenderingPrimitive();
    void CompileAllShaders();
    
    const ServiceLocator& mServiceLocator;
    SDL_Window* mSdlWindow;
    SDL_GLContext mSdlGLContext;
    
    bool mRunning;
    
    int mRenderableAreaWidth;
    int mRenderableAreaHeight;
    
    GLuint mVAO;
    GLuint mVBO;

    std::map<std::string, std::unique_ptr<Shader>> mShaders;
    std::string mCurrentShaderUsed;
};


#endif /* CoreRenderingService_h */
