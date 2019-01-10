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
public:
    CoreEngineService();
    ~CoreEngineService();
    
    bool InitializeContext();
    void GameLoop(std::function<void(const float)> clientUpdateMethod);
    
private:
    void ParseEvent();
    
    using SDL_GLContext = void*;
    
    SDL_Window* mSdlWindow;
    SDL_GLContext mSdlGlContext;
    int mRenderableAreaWidth;
    int mRenderableAreaHeight;
};


#endif /* CoreEngineService_h */
