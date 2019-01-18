//
//  CoreRenderingService.cpp
//  Hardcore2D
//
//  Created by Alex Koukoulas on 10/01/2019.
//

#include "CoreRenderingService.h"
#include "Shader.h"
#include "../ServiceLocator.h"
#include "../util/StringUtils.h"
#include "../util/ShaderUtils.h"
#include "../util/FileUtils.h"
#include "../util/Logging.h"
#include "../util/SDLMessageBoxUtils.h"
#include "../gl/Context.h"
#include "../resources/ResourceManager.h"
#include "../components/TransformationComponent.h"
#include "../components/EntityComponentManager.h"
#include "../components/AnimationComponent.h"
#include "../components/ShaderComponent.h"
#include "../resources/TextureResource.h"
#include "../resources/TextFileResource.h"

#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const char* SHADER_DIRECTORY = "shaders/";

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
    , mRenderableAreaWidth(0.0f)
    , mRenderableAreaHeight(0.0f)
    , mVAO(0U)
    , mVBO(0U)
    , mCurrentShaderUsed("basic")
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
    if (!InitializeContext()) return false;
    InitializeRenderingPrimitive();
    CompileAllShaders();
    
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
        GL_CHECK(glBindVertexArray(mVAO));
        
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
		
        // Update client
        clientUpdateMethod(dt);
       
        // Swap window buffers
        SDL_GL_SwapWindow(mSdlWindow);
    }
}

void CoreRenderingService::RenderEntity(const EntityId entityId)
{
    auto& entityComponentManager = mServiceLocator.ResolveService<EntityComponentManager>();
    
    if (entityComponentManager.HasComponent<ShaderComponent>(entityId))
    {
        const auto& shaderComponent = entityComponentManager.GetComponent<ShaderComponent>(entityId);
        mCurrentShaderUsed = shaderComponent.GetShaderName();
        GL_CHECK(glUseProgram(mShaders[mCurrentShaderUsed]->GetShaderId()));
    }
    
    if (entityComponentManager.HasComponent<AnimationComponent>(entityId))
    {
        const auto& animationComponent = entityComponentManager.GetComponent<AnimationComponent>(entityId);
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, animationComponent.GetCurrentFrameResourceId()));
    }
    
    if (entityComponentManager.HasComponent<TransformationComponent>(entityId))
    {
        const auto& transformationComponent = entityComponentManager.GetComponent<TransformationComponent>(entityId);
        
        // Todo move world matrix construction elsewhere
        glm::mat4 worldMatrix(1.0f);
        worldMatrix = glm::translate(worldMatrix, transformationComponent.mTranslation);
        worldMatrix = glm::rotate(worldMatrix, transformationComponent.mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        worldMatrix = glm::rotate(worldMatrix, transformationComponent.mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        worldMatrix = glm::rotate(worldMatrix, transformationComponent.mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        worldMatrix = glm::scale(worldMatrix, transformationComponent.mScale);       		
		
        GL_CHECK(glUniformMatrix4fv(mShaders[mCurrentShaderUsed]->GetUniformNamesToLocations().at("world"), 1, GL_FALSE, (GLfloat*) &worldMatrix));		
    }
    
	SetCommonShaderUniformsForEntity(entityId);

    GL_CHECK(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
}

float CoreRenderingService::GetRenderableWidth() const
{
    return mRenderableAreaWidth;
}

float CoreRenderingService::GetRenderableHeight() const
{
    return mRenderableAreaHeight;
}

bool CoreRenderingService::InitializeContext()
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
	int rendWidth = 0;
	int rendHeight = 0;
    SDL_GL_GetDrawableSize(mSdlWindow, &rendWidth, &rendHeight);
	mRenderableAreaWidth = static_cast<float>(rendWidth);
	mRenderableAreaHeight = static_cast<float>(rendHeight);

    // Log GL driver info
    Log(LogType::INFO, "Vendor     : %s", GL_NO_CHECK(glGetString(GL_VENDOR)));
    Log(LogType::INFO, "Renderer   : %s", GL_NO_CHECK(glGetString(GL_RENDERER)));
    Log(LogType::INFO, "Version    : %s", GL_NO_CHECK(glGetString(GL_VERSION)));
    
    GL_CHECK(glClearColor(1.0f, 1.0f, 1.0, 1.0f));
    GL_CHECK(glEnable(GL_BLEND));
    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    return true;
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

void CoreRenderingService::CompileAllShaders()
{	
	auto& resourceManager = mServiceLocator.ResolveService<ResourceManager>();

	auto currentVertexShaderId = 0;
	auto currentFragmentShaderId = 0;
	auto currentProgramId = 0;
    std::vector<std::string> shaderUniformNames;
    
    const auto shaderFileNames = GetAllFilenamesInDirectory(resourceManager.GetRootResourceDirectory() + SHADER_DIRECTORY);
    
    for (const auto fileName: shaderFileNames)
	{
		if (GetFileExtension(fileName) == "vs")
		{
			// Generate vertex shader id
			currentVertexShaderId = GL_NO_CHECK(glCreateShader(GL_VERTEX_SHADER));

			// Get vertex shader contents
			const auto vertexShaderFileResourceId = resourceManager.LoadResource(SHADER_DIRECTORY + fileName);
			const auto& vertexShaderFileResource = resourceManager.GetResource<TextFileResource&>(vertexShaderFileResourceId);
			const char* vertexShaderFileContents = vertexShaderFileResource.GetContents().c_str();
            
			// Compile vertex shader
			GL_CHECK(glShaderSource(currentVertexShaderId, 1, &vertexShaderFileContents, NULL));
			GL_CHECK(glCompileShader(currentVertexShaderId));

			// Check vertex shader compilation
			std::string infoLog;
			GLint infoLogLength;
			GL_CHECK(glGetShaderiv(currentVertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength));
			if (infoLogLength > 0)
			{
				infoLog.clear();
				infoLog.reserve(infoLogLength);
				GL_CHECK(glGetShaderInfoLog(currentVertexShaderId, infoLogLength, NULL, &infoLog[0]));
				Log(LogType::INFO, "While compiling vertex shader:\n%s", infoLog.c_str());
			}

			// Link shader program
			currentProgramId = GL_NO_CHECK(glCreateProgram());
			GL_CHECK(glAttachShader(currentProgramId, currentVertexShaderId));
			GL_CHECK(glAttachShader(currentProgramId, currentFragmentShaderId));
			GL_CHECK(glLinkProgram(currentProgramId));

#ifndef _WIN32
			glGetProgramiv(currentProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				infoLog.clear();
				infoLog.reserve(infoLogLength);
				GL_CHECK(glGetProgramInfoLog(currentProgramId, infoLogLength, NULL, &infoLog[0]));
				Log(LogType::INFO, "While linking shader:\n%s", infoLog.c_str());
			}
#endif

			GL_CHECK(glValidateProgram(currentProgramId));

#ifndef _WIN32
            GLint status;
			GL_CHECK(glGetProgramiv(currentProgramId, GL_VALIDATE_STATUS, &status));
			glGetProgramiv(currentProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				infoLog.clear();
				infoLog.reserve(infoLogLength);
				GL_CHECK(glGetProgramInfoLog(currentProgramId, infoLogLength, NULL, &infoLog[0]));
				Log(LogType::INFO, "While validating shader:\n%s", infoLog.c_str());
			}
#endif

			// Destroy intermediate compiled shaders
			GL_CHECK(glDetachShader(currentProgramId, currentVertexShaderId));
			GL_CHECK(glDetachShader(currentProgramId, currentFragmentShaderId));
			GL_CHECK(glDeleteShader(currentVertexShaderId));
			GL_CHECK(glDeleteShader(currentFragmentShaderId));
            
            // Extract vertex shader uniform names
            const auto uniformNames = GetUniformNames(vertexShaderFileResource.GetContents());
            shaderUniformNames.insert(shaderUniformNames.end(), uniformNames.begin(), uniformNames.end());
            
            // Extract uniform locations
            std::map<std::string, GLuint> shaderUniformNamesToLocations;
            for (const auto uniformName: shaderUniformNames)
            {
                shaderUniformNamesToLocations[uniformName] = GL_NO_CHECK(glGetUniformLocation(currentProgramId, uniformName.c_str()));
            }
            
			// Create shader object
			const auto programName = GetFileNameWithoutExtension(fileName);
            
            // Save shader
            mShaders[programName] = std::make_unique<Shader>(currentProgramId, shaderUniformNamesToLocations);
		}
		else
		{
			// Generate fragment shader id
			currentFragmentShaderId = GL_NO_CHECK(glCreateShader(GL_FRAGMENT_SHADER));

			// Get fragment shader contents			
			const auto fragmentShaderFileResourceId = resourceManager.LoadResource(SHADER_DIRECTORY + fileName);
			const auto& fragmentShaderFileResource = resourceManager.GetResource<TextFileResource&>(fragmentShaderFileResourceId);
			const char* fragmentShaderFileContents = fragmentShaderFileResource.GetContents().c_str();
			
			GL_CHECK(glShaderSource(currentFragmentShaderId, 1, &fragmentShaderFileContents, NULL));
			GL_CHECK(glCompileShader(currentFragmentShaderId));

			std::string infoLog;
			GLint infoLogLength;
			GL_CHECK(glGetShaderiv(currentFragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength));
			if (infoLogLength > 0)
			{
				infoLog.clear();
				infoLog.reserve(infoLogLength);
				GL_CHECK(glGetShaderInfoLog(currentFragmentShaderId, infoLogLength, NULL, &infoLog[0]));
				Log(LogType::INFO, "While compiling fragment shader:\n%s", infoLog.c_str());
			}
            
            // Extract fragment shader uniform names
            shaderUniformNames.clear();
            const auto uniformNames = GetUniformNames(fragmentShaderFileResource.GetContents());
            shaderUniformNames.insert(shaderUniformNames.end(), uniformNames.begin(), uniformNames.end());
		}		
	}
}

void CoreRenderingService::SetCommonShaderUniformsForEntity(const EntityId)
{
	//auto& entityComponentManager = mServiceLocator.ResolveService<EntityComponentManager>();
	const auto& shaderUniforms = mShaders[mCurrentShaderUsed]->GetUniformNamesToLocations();

	if (shaderUniforms.count("view") != 0)
	{		
		glm::mat4 viewMatrix = glm::lookAtLH(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		GL_CHECK(glUniformMatrix4fv(mShaders[mCurrentShaderUsed]->GetUniformNamesToLocations().at("view"), 1, GL_FALSE, (GLfloat*)&viewMatrix));
	}	
}