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

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		fprintf(stderr, "Warning: Linear texture filtering not enabled!\n");
		printf("Warning: Linear texture filtering not enabled!\n");
	}

	window = SDL_CreateWindow("Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		fprintf(stderr, "Cannot create window. Error: %s\n", SDL_GetError());
		exit(-1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED/* | SDL_RENDERER_PRESENTVSYNC*/);
	if (renderer == NULL)
	{
		fprintf(stderr, "Cannot create renderer. Error: %s\n", SDL_GetError());
		exit(-1);
	}

	//Init PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		fprintf(stderr, "Cannot init PNG loading. Error: %s\n", IMG_GetError());
		exit(-1);
	}

	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

void Window::centerCamera(const int x, const int y, Map & map)
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

void Window::close()
{
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
}

Window* winptr;
