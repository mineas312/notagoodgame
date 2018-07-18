#include "stdafx.h"

#include "Window.h"

// -Initializes window, renderer

void Window::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Cannot init SDL. Error: %s\n", SDL_GetError());
		exit(-1);
	}

	window = SDL_CreateWindow("Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		fprintf(stderr, "Cannot create window. Error: %s\n", SDL_GetError());
		exit(-1);
	}

	setOpenGLAtrributes();
	context = SDL_GL_CreateContext(window);
	if (context == NULL)
	{
		fprintf(stderr, "Cannot create renderer. Error: %s\n", SDL_GetError());
		exit(-1);
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

	// Multithreaded shader compilation
	const uint32_t maxThreads = std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 1;
	if (GLAD_GL_ARB_parallel_shader_compile)
		glMaxShaderCompilerThreadsARB(maxThreads);
	else if (GLAD_GL_KHR_parallel_shader_compile)
		glMaxShaderCompilerThreadsKHR(maxThreads);

	// Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		fprintf(stderr, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}
	//Init PNG loading
	const int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		fprintf(stderr, "Cannot init PNG loading. Error: %s\n", IMG_GetError());
		exit(-1);
	}
	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

void Window::centerCamera(const int x, const int y, const Map & map) noexcept
{
	camera.x = x - SCREEN_WIDTH / 2;
	camera.y = y - SCREEN_HEIGHT / 2;

	if (camera.x < 0)
		camera.x = 0;

	if (camera.x > map.width - camera.w)
		camera.x = map.width - camera.w;

	if (camera.y < 0)
		camera.y = 0;

	if (camera.y > map.height - camera.h)
		camera.y = map.height - camera.h;
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
#endif // !_DEBUG
}

Window* winptr;
