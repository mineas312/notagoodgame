#pragma once
#include "Texture.h"

class Window
{
public:
	Window(int width, int height)
	{
		window = NULL;
		screen = NULL;
		SCREEN_WIDTH = width;
		SCREEN_HEIGHT = height;
		camera = { 0, 0, SCREEN_WIDTH - 256, SCREEN_HEIGHT};
		quit = false;
		xSpeed = 3;
		ySpeed = 3;
		rxSpeed = 0.0f;
		rySpeed = 0.0f;
	}
	bool init()
	{
		//Initialize all SDL subsystems
		if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		{
			return false;
		}
		window = SDL_CreateWindow("Level Designer. Current Tile ID: 0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			return false;
		}
		g_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (g_renderer == NULL)
		{
			printf("Cannot create renderer");
			return false;
		}
		SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);

		return true;
	}
	void set_camera(int mapWidth, int mapHeight, Uint32 fps)
	{
		if (fps == 0)
			return;

		//Mouse offsets
		int x = 0, y = 0;

		//Get mouse offsets
		SDL_GetMouseState(&x, &y);

		//Move camera to the left if needed
		if (x < TILE_WIDTH)
		{
			camera.x -= 800 / fps;
		}

		//Move camera to the right if needed
		if ((x > SCREEN_WIDTH - TILE_WIDTH - 256) && (x < SCREEN_WIDTH - 256))
		{
			camera.x += 800 / fps;
		}

		//Move camera up if needed
		if (y < TILE_WIDTH)
		{
			camera.y -= 800 / fps;
		}

		//Move camera down if needed
		if (y > SCREEN_HEIGHT - TILE_WIDTH)
		{
			camera.y += 800 / fps;
		}

		//Keep the camera in bounds.
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		if (camera.x > mapWidth - camera.w)
		{
			camera.x = mapWidth - camera.w;
		}
		if (camera.y > mapHeight - camera.h)
		{
			camera.y = mapHeight - camera.h;
		}
	}
private:
	SDL_Surface * screen;
	bool quit;
	int xSpeed;
	int ySpeed;
	float rxSpeed;
	float rySpeed;
public:
	SDL_Event e;
	SDL_Rect camera;
	SDL_Window * window;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
};

Window * wptr;