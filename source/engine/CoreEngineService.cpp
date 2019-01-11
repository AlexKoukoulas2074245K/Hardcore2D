//
//  CoreEngineService.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "CoreEngineService.h"
#include "../util/Logging.h"
#include "../util/SDLCommonUtils.h"
#include "../wingl/Context.h"

CoreEngineService::CoreEngineService()
    : mSdlWindow(nullptr)
    , mSdlGlContext(nullptr)
    , mRenderableAreaWidth(0)
    , mRenderableAreaHeight(0)
{
    
}

CoreEngineService::~CoreEngineService()
{
    SDL_GL_MakeCurrent(nullptr, nullptr);
    SDL_GL_DeleteContext(mSdlGlContext);
    SDL_DestroyWindow(mSdlWindow);
    SDL_Quit();
}

bool CoreEngineService::InitializeEngine()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
    {
        ShowMessageBox(SDL_MESSAGEBOX_ERROR, "Error initializing SDL", "An error has occurred while trying to initialize SDL");
        return false;
    }
    
    // Set SDL GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#if DEMO_ENABLE_MULTISAMPLE
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
#endif
    
    // Create SDL window
    mSdlWindow = SDL_CreateWindow("CMakeDemo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 440, SDL_WINDOW_OPENGL);
    
    if (mSdlWindow == nullptr)
    {
        ShowMessageBox(SDL_MESSAGEBOX_ERROR, "Error creating SDL window", "An error has occurred while trying to create an SDL_Window");
        return false;
    }
    
    // Make window resizable
    SDL_SetWindowResizable(mSdlWindow, SDL_TRUE);
    
    SDL_ShowWindow(mSdlWindow);
    
    // Create SDL GL context
    mSdlGlContext = SDL_GL_CreateContext(mSdlWindow);
    if (mSdlGlContext == nullptr)
    {
        ShowMessageBox(SDL_MESSAGEBOX_ERROR, "Error creating SDL context", "An error has occurred while trying to create an SDL_Context");
        return false;
    }
    
    SDL_GL_MakeCurrent(mSdlWindow, mSdlGlContext);
    SDL_GL_SetSwapInterval(1);
    
#ifdef _WIN32
    // Initialize GLES2 function table
    glFuncTable.initialize();
#endif
    
    // Get render buffer width/height
    SDL_GL_GetDrawableSize(mSdlWindow, &mRenderableAreaWidth, &mRenderableAreaHeight);
    
    // Log GL driver info
    Log(LogType::INFO, "Vendor     : %s", GL_NO_CHECK(GetString(GL_VENDOR)));
    Log(LogType::INFO, "Renderer   : %s", GL_NO_CHECK(GetString(GL_RENDERER)));
    Log(LogType::INFO, "Version    : %s", GL_NO_CHECK(GetString(GL_VERSION)));
    Log(LogType::INFO, "Extensions : %s", GL_NO_CHECK(GetString(GL_EXTENSIONS)));
    
    return true;
}

void CoreEngineService::GameLoop(std::function<void(const float)> clientUpdateMethod)
{
    SDL_Event event;
    float elapsedTicks = 0.0f;
    mRunning = true;
    
    while(mRunning)
    {
        SDL_GL_SwapWindow(mSdlWindow);
        GL_CHECK(Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GL_CHECK(ClearColor(1.0f, 1.0f, 1.0, 1.0f));
        
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: mRunning = false; break;
                case SDL_WINDOWEVENT:
                {
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                        {
                            SDL_GL_GetDrawableSize(mSdlWindow, &mRenderableAreaWidth, &mRenderableAreaHeight);
                        }
                    }
                } break;
            }
        }
        
        const auto currentTicks = static_cast<float>(SDL_GetTicks());
        auto lastFrameTicks = currentTicks - elapsedTicks;
        elapsedTicks = currentTicks;
        const auto dt = lastFrameTicks * 0.001f;
        
        clientUpdateMethod(dt);
    }
}
