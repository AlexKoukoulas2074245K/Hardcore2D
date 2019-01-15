//
//  CoreEngineService.h
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#ifndef CoreEngineService_h
#define CoreEngineService_h

#include "../IService.h"

#include <functional>

struct SDL_Window;
struct SDL_Context;

class CoreEngineService final: public IService
{
    friend class App;
public:
    ~CoreEngineService();
    
    bool InitializeEngine();
    void GameLoop(std::function<void(const float)> clientUpdateMethod);
    
private:
    using SDL_GLContext = void*;
    
    CoreEngineService();
    
    void ParseEvents();
    
    SDL_Window* mSdlWindow;
    SDL_GLContext mSdlGlContext;
    bool mRunning;
    int mRenderableAreaWidth;
    int mRenderableAreaHeight;
};


#endif /* CoreEngineService_h */
