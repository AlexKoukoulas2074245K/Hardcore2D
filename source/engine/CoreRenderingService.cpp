//
//  CoreRenderingService.cpp
//  Ninja
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "CoreRenderingService.h"
#include "../ServiceLocator.h"
#include "../util/Logging.h"
#include "../util/SDLMessageBoxUtils.h"
#include "../gl/Context.h"
#include "../resources/ResourceManager.h"
#include "../resources/TextureResource.h"
#include "../resources/TextFileResource.h"

#include <string>

static const GLfloat QUAD_VERTICES[] =
{
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
};

CoreRenderingService::CoreRenderingService(const ServiceLocator& serviceLocator)
    : mServiceLocator(serviceLocator)
    , mSdlWindow(nullptr)
    , mSdlGLContext(nullptr)
    , mRenderableAreaWidth(0)
    , mRenderableAreaHeight(0)
    , mVAO(0U)
    , mVBO(0U)
    , mDefaultShaderId(0U)
{
    
}

CoreRenderingService::~CoreRenderingService()
{
    GL_CHECK(glDeleteVertexArrays(1, &mVAO));
    GL_CHECK(glDeleteBuffers(1, &mVBO));
    
    SDL_GL_MakeCurrent(nullptr, nullptr);
    SDL_GL_DeleteContext(mSdlGLContext);
    SDL_DestroyWindow(mSdlWindow);
    SDL_Quit();
}

bool CoreRenderingService::InitializeEngine()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
    {
        ShowMessageBox(SDL_MESSAGEBOX_ERROR, "Error initializing SDL", "An error has occurred while trying to initialize SDL");
        return false;
    }
    
    // Set SDL GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
#if DEMO_ENABLE_MULTISAMPLE
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
#endif
    
    // Get Current Display Mode Resolution
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    
    const auto defaultWindowWidth = static_cast<int>(displayMode.w * 0.66f);
    const auto defaultWindowHeight = static_cast<int>(displayMode.h * 0.66f);
    
    // Create SDL window
    mSdlWindow = SDL_CreateWindow("CMakeDemo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, defaultWindowWidth, defaultWindowHeight, SDL_WINDOW_OPENGL);
    
    if (mSdlWindow == nullptr)
    {
        ShowMessageBox(SDL_MESSAGEBOX_ERROR, "Error creating SDL window", "An error has occurred while trying to create an SDL_Window");
        return false;
    }
    
    // Make window non-resizable
    SDL_SetWindowResizable(mSdlWindow, SDL_FALSE);
    
    SDL_ShowWindow(mSdlWindow);
    
    // Create SDL GL context
    mSdlGLContext = SDL_GL_CreateContext(mSdlWindow);
    if (mSdlGLContext == nullptr)
    {
        ShowMessageBox(SDL_MESSAGEBOX_ERROR, "Error creating SDL context", "An error has occurred while trying to create an SDL_Context");
        return false;
    }
    
    SDL_GL_MakeCurrent(mSdlWindow, mSdlGLContext);
    SDL_GL_SetSwapInterval(1);
    
#ifdef _WIN32
    // Initialize GLES2 function table
    glFuncTable.initialize();
#endif
    
    // Get render buffer width/height
    SDL_GL_GetDrawableSize(mSdlWindow, &mRenderableAreaWidth, &mRenderableAreaHeight);
    
    // Log GL driver info
    Log(LogType::INFO, "Vendor     : %s", GL_NO_CHECK(glGetString(GL_VENDOR)));
    Log(LogType::INFO, "Renderer   : %s", GL_NO_CHECK(glGetString(GL_RENDERER)));
    Log(LogType::INFO, "Version    : %s", GL_NO_CHECK(glGetString(GL_VERSION)));
    
    GL_CHECK(glClearColor(1.0f, 1.0f, 1.0, 1.0f));
    
    InitializeRenderingPrimitive();
    CompileShaders();
    
    return true;
}

void CoreRenderingService::GameLoop(std::function<void(const float)> clientUpdateMethod)
{
    SDL_Event event;
    float elapsedTicks = 0.0f;
    mRunning = true;
    
    while(mRunning)
    {
        // Clear viewport
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        
        // Poll events
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: mRunning = false; break;
            }
        }
        
        // Calculate frame delta
        const auto currentTicks = static_cast<float>(SDL_GetTicks());
        auto lastFrameTicks = currentTicks - elapsedTicks;
        elapsedTicks = currentTicks;
        const auto dt = lastFrameTicks * 0.001f;
        
        clientUpdateMethod(dt);
        
        auto& resourceManager = mServiceLocator.ResolveService<ResourceManager>();
        auto& resource = static_cast<TextureResource&>(resourceManager.GetResource("jungle-sky.png"));
        
        
        GL_CHECK(glUseProgram(mDefaultShaderId));
        GL_CHECK(glBindVertexArray(mVAO));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, resource.GetGLTextureId()));
        GL_CHECK(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
        
        SDL_GL_SwapWindow(mSdlWindow);
    }
}

int CoreRenderingService::GetRenderableWidth() const
{
    return mRenderableAreaWidth;
}

int CoreRenderingService::GetRenderableHeight() const
{
    return mRenderableAreaHeight;
}

void CoreRenderingService::InitializeRenderingPrimitive()
{
    // Create VAO & VBO
    GL_CHECK(glGenVertexArrays(1, &mVAO));
    GL_CHECK(glGenBuffers(1, &mVBO));
    
    // Bind VAO & VBO
    GL_CHECK(glBindVertexArray(mVAO));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
    
    // Fill in VBO
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW));
    
    // Set Attribute Pointers
    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QUAD_VERTICES)/4, nullptr));
    GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QUAD_VERTICES)/4, (void*)(sizeof(float) * 3)));
    
    // Enable vertex attribute arrays
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glEnableVertexAttribArray(1));
}

void CoreRenderingService::CompileShaders()
{
    
    // Compile vertex shader
    GLuint vertexShader = GL_NO_CHECK(glCreateShader(GL_VERTEX_SHADER));
    
    auto& resourceManager = mServiceLocator.ResolveService<ResourceManager>();
    const auto vertexShaderFileResourceId = resourceManager.LoadResource("shaders/basic.vs");
    const auto& vertexShaderFileResource = static_cast<TextFileResource&>(resourceManager.GetResource(vertexShaderFileResourceId));
    
    const char* vertexShaderFileContents = vertexShaderFileResource.GetContents().c_str();
    GL_CHECK(glShaderSource(vertexShader, 1, &vertexShaderFileContents, NULL));
    GL_CHECK(glCompileShader(vertexShader));
    
    GLint status;
    GL_CHECK(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status));
    
    std::string infoLog;
    GLint infoLogLength;
    GL_CHECK(glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength));
    if (infoLogLength > 0)
    {
        infoLog.clear();
        infoLog.reserve(infoLogLength);
        GL_CHECK(glGetShaderInfoLog(vertexShader, infoLogLength, NULL, &infoLog[0]));
        Log(LogType::INFO, "While compiling vertex shader:\n%s", infoLog.c_str());
    }
    
    // Compile fragment shader
    GLuint fragmentShader = GL_NO_CHECK(glCreateShader(GL_FRAGMENT_SHADER));
    const auto fragmentShaderFileResourceId = resourceManager.LoadResource("shaders/basic.fs");
    const auto& fragmentShaderFileResource = static_cast<TextFileResource&>(resourceManager.GetResource(fragmentShaderFileResourceId));
    
    const char* fragmentShaderFileContents = fragmentShaderFileResource.GetContents().c_str();
    GL_CHECK(glShaderSource(fragmentShader, 1, &fragmentShaderFileContents, NULL));
    GL_CHECK(glCompileShader(fragmentShader));
    GL_CHECK(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status));
    
    GL_CHECK(glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength));
    if (infoLogLength > 0)
    {
        infoLog.clear();
        infoLog.reserve(infoLogLength);
        GL_CHECK(glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, &infoLog[0]));
        Log(LogType::INFO, "While compiling fragment shader:\n%s", infoLog.c_str());
    }
    
    // Link shader program
    mDefaultShaderId = GL_NO_CHECK(glCreateProgram());
    GL_CHECK(glAttachShader(mDefaultShaderId, vertexShader));
    GL_CHECK(glAttachShader(mDefaultShaderId, fragmentShader));
    GL_CHECK(glLinkProgram(mDefaultShaderId));
    
#ifndef _WIN32
	GL_CHECK(glGetProgramiv(mDefaultShaderId, GL_LINK_STATUS, &status));
    
    glGetProgramiv(mDefaultShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        infoLog.clear();
        infoLog.reserve(infoLogLength);
        GL_CHECK(glGetProgramInfoLog(mDefaultShaderId, infoLogLength, NULL, &infoLog[0]));
        Log(LogType::INFO, "While linking shader:\n%s", infoLog.c_str());
    }
#endif

    GL_CHECK(glValidateProgram(mDefaultShaderId));

#ifndef _WIN32
    GL_CHECK(glGetProgramiv(mDefaultShaderId, GL_VALIDATE_STATUS, &status));    
    glGetProgramiv(mDefaultShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        infoLog.clear();
        infoLog.reserve(infoLogLength);
        GL_CHECK(glGetProgramInfoLog(mDefaultShaderId, infoLogLength, NULL, &infoLog[0]));
        Log(LogType::INFO, "While validating shader:\n%s", infoLog.c_str());
    }
#endif

    // Destroy intermediate compiled shaders
    GL_CHECK(glDetachShader(mDefaultShaderId, vertexShader));
    GL_CHECK(glDetachShader(mDefaultShaderId, fragmentShader));
    GL_CHECK(glDeleteShader(vertexShader));
    GL_CHECK(glDeleteShader(fragmentShader));
}
