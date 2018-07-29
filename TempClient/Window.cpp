#include "stdafx.h"

#include "Window.h"
#include "OGLDebug.h"

// -Initializes window

void Window::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Cannot init SDL. Error: %s\n", SDL_GetError());
		fprintf(stderr, "Cannot init SDL. Error: %s\n", SDL_GetError());
		std::exit(-1);
	}

	window = SDL_CreateWindow("Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		printf("Cannot create window. Error: %s\n", SDL_GetError());
		fprintf(stderr, "Cannot create window. Error: %s\n", SDL_GetError());
		std::exit(-1);
	}

	setOpenGLAtrributes();
	context = SDL_GL_CreateContext(window);
	if (context == NULL)
	{
		printf("Cannot create context.Error: %s\n", SDL_GetError());
		fprintf(stderr, "Cannot create context. Error: %s\n", SDL_GetError());
		std::exit(-1);
	}

	// glad
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		printf("Error initializing glad");
		fprintf(stderr, "Error initializing glad");
	}

	// Multisampling
#ifdef MSAA
	glEnable(GL_MULTISAMPLE);
#else
	glDisable(GL_MULTISAMPLE);
#endif

	// Alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Multithreaded shader compilation
	const uint32_t maxThreads = std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 1;
	if (GLAD_GL_ARB_parallel_shader_compile)
		glMaxShaderCompilerThreadsARB(maxThreads);
	else if (GLAD_GL_KHR_parallel_shader_compile)
		glMaxShaderCompilerThreadsKHR(maxThreads);

#ifdef _DEBUG
	printf("OpenGL debug output is enabled\n\n");
	fprintf(stderr, "OpenGL debug output is enabled\n\n");
		
	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	// Enable all messages and test output
	glDebugMessageCallback(oglDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, 21, "Debug test message\0");
#endif

	SDL_GL_SetSwapInterval(0);

	//Init PNG loading
	const int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		fprintf(stderr, "Cannot init PNG loading. Error: %s\n", IMG_GetError());
		exit(-1);
	}
}

void Window::close() noexcept
{
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_GL_DeleteContext(context);
	context = NULL;
}

void Window::setOpenGLAtrributes() noexcept
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#ifdef MSAA
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
#endif
#ifndef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR, 1);
#endif
}

Window* winptr;
