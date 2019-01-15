//
//  CoreRenderingService.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef CoreRenderingService_h
#define CoreRenderingService_h

#include "../IService.h"

#include <functional>
#include <map>

struct SDL_Window;
struct SDL_Context;

class ServiceLocator;

class CoreRenderingService final: public IService
{
    friend class App;
public:
	enum class ShaderType
	{

	};

    ~CoreRenderingService();
    
    bool InitializeEngine();
    void GameLoop(std::function<void(const float)> clientUpdateMethod);
    
    int GetRenderableWidth() const;
    int GetRenderableHeight() const;
    
private:
    using SDL_GLContext = void*;
    using GLuint = unsigned int;
    
    CoreRenderingService(const ServiceLocator&);
    
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

	std::map<std::string, GLuint> mShaders;
};


#endif /* CoreRenderingService_h */
