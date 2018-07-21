#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>

TTF_Font * font;
SDL_Renderer * g_renderer;

class Texture
{
public:
	Texture()
	{
		texture = NULL;
		width = 0;
		height = 0;
	}
	bool loadTexture(const char* path)
	{
		bool success = true;
		free();
		SDL_Surface* loadedSurface = IMG_Load(path);
		if (loadedSurface == NULL)
		{
			fprintf(stderr, "Cannot load image %s. SDL_image Error: %s\n", path, IMG_GetError());
			printf("Cannot load image %s. SDL_image Error: %s\n", path, IMG_GetError());
			SDL_FreeSurface(loadedSurface);
			success = false;
		}
		else
		{
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
			texture = SDL_CreateTextureFromSurface(g_renderer, loadedSurface);
			width = loadedSurface->w;
			height = loadedSurface->h;
			if (texture == NULL)
			{
				fprintf(stderr, "Cannot create texture from %s. SDL Error: %s\n", path, SDL_GetError());
				printf("Cannot create texture from %s. SDL Error: %s\n", path, SDL_GetError());
				success = false;
			}
		}
		SDL_FreeSurface(loadedSurface);
		return success;
	}
	bool loadFromRenderedText(char* textureText, SDL_Color textColor)
	{
		bool success = true;
		free();
		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText, textColor);
		if (textSurface != NULL)
		{
			//Create texture from surface pixels
			texture = SDL_CreateTextureFromSurface(g_renderer, textSurface);
			if (texture == NULL)
			{
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Get image dimensions
				width = textSurface->w;
				height = textSurface->h;
			}

			//Get rid of old surface
			SDL_FreeSurface(textSurface);
		}
		else
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}
		return success;
	}
	void free()
	{
		if (texture != NULL)
		{
			SDL_DestroyTexture(texture);
			texture == NULL;
			width = 0;
			height = 0;
		}
	}
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		SDL_Rect quad = { x, y, width, height };
		if (clip != NULL)
		{
			quad.w = clip->w;
			quad.h = clip->h;
		}
		SDL_RenderCopyEx(g_renderer, texture, clip, &quad, angle, center, flip);
	}
public:
	SDL_Texture * texture;
	int width;
	int height;
};

class Window
{
public:
	Window()
	{
		window = NULL;
		screen = NULL;
		SCREEN_WIDTH = 640;
		SCREEN_HEIGHT = 480;
		currentType = 0;
		tiles = NULL;
		tileSheet = NULL;
		camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		quit = false;
	}
	bool init()
	{
		//Initialize all SDL subsystems
		if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		{
			return false;
		}
		window = SDL_CreateWindow("Level Designer. Current Tile: Red Tile", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			return false;
		}
		g_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (g_renderer == NULL)
		{
			printf("Cannot create renderer");
			return false;
		}

		m.setMap(1280, 960);

		tiles = new Tile[m.getTotalTiles()];
		//If everything initialized fine
		return true;
	}
	void set_camera(int mapWidth, int mapHeight)
	{
		//Mouse offsets
		int x = 0, y = 0;

		//Get mouse offsets
		SDL_GetMouseState(&x, &y);

		//Move camera to the left if needed
		if (x < TILE_WIDTH)
		{
			camera.x -= 20;
		}

		//Move camera to the right if needed
		if (x > SCREEN_WIDTH - TILE_WIDTH)
		{
			camera.x += 20;
		}

		//Move camera up if needed
		if (y < TILE_WIDTH)
		{
			camera.y -= 20;
		}

		//Move camera down if needed
		if (y > SCREEN_HEIGHT - TILE_WIDTH)
		{
			camera.y += 20;
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
	void save_tiles()
	{
		//Open the map
		std::ofstream map("lazy.map");

		//Go through the tiles
		for (int t = 0; t < m.getTotalTiles(); t++)
		{
			//Write tile type to file
			map << tiles[t].get_type() << " ";
		}

		//Close the file
		map.close();
	}
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	SDL_Window * window;
	SDL_Surface * screen;
	bool quit;
public:
	SDL_Event e;
	SDL_Rect camera;
};

Window * wptr;