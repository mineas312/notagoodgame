#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Map.h"

class Window
{
public:
	// -Sets width and height of window
	Window(const int width, const int height) : SCREEN_WIDTH{ width }, SCREEN_HEIGHT{ height }, window{ NULL }, renderer{ NULL }
	{}

	// -Initializes window, renderer
	void init()
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

		window = SDL_CreateWindow("Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
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

	void centerCamera(const int x, const int y, Map& map)
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

	void close()
	{
		SDL_DestroyWindow(window);
		window = NULL;
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

public:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect camera;
};

Window* winptr;
