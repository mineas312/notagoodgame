#pragma once
#include "Engine.h"

class Graphics
{
public:
	Graphics()
	{
		window = NULL;
		renderer = NULL;
	}
	void init()
	{
		enptr = new Engine();
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			fprintf(stderr, "Cannot init SDL. Error: %s\n", SDL_GetError());
			exit(-1);
		}
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		window = SDL_CreateWindow("Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			fprintf(stderr, "Cannot create window. Error: %s\n", SDL_GetError());
			exit(-1);
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
		loadMedia();
	}
	void render()
	{
		centerCamera();

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		for (int i = 0; i < enptr->maps[enptr->currentMap].MAP_TILES; i++)
		{
			enptr->maps[enptr->currentMap].tileSet[i].render(renderer, camera);
		}

		//Main character rendering
		textures[CHARACTER].render(renderer, enptr->character.box.x - camera.x, enptr->character.box.y - camera.y);
		
		//Other entities rendering
		for (Entity en : enptr->entities)
			textures[CHARACTER].render(renderer, en.box.x - camera.x, en.box.y - camera.y);

		SDL_RenderPresent(renderer);
	}
private:
	bool loadMedia()
	{
		bool success = true;

		if (!textures[CHARACTER].loadTexture("res/link.png", renderer))
			success = false;
		if (!tilesTexutre.loadTexture("res/tiles.png", renderer))
			success = false;
		return success;
	}
	void centerCamera()
	{
		// Center the camera over the character TODO Center of the texture
		camera.x = (enptr->character.box.x) - SCREEN_WIDTH / 2;
		camera.y = (enptr->character.box.y) - SCREEN_HEIGHT / 2;

		//Keep camera in bounds
		if (camera.x < 0)
			camera.x = 0;
		if (camera.y < 0)
			camera.y = 0;
		if (camera.x > enptr->maps[enptr->currentMap].MAP_WIDTH - camera.w)
			camera.x = enptr->maps[enptr->currentMap].MAP_WIDTH - camera.w;
		if (camera.y > enptr->maps[enptr->currentMap].MAP_HEIGHT - camera.h)
			camera.y = enptr->maps[enptr->currentMap].MAP_HEIGHT - camera.h;
	}
public:
	SDL_Event e;
private:
	/// Window components
	SDL_Window * window;
	SDL_Renderer * renderer;
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	/// Game components
	Texture textures[TOTAL_TEXTURES];
};