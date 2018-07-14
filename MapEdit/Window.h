#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>

TTF_Font * font;

class Texture
{
public:
	Texture()
	{
		texture = NULL;
		width = 0;
		height = 0;
	}
	bool loadTexture(const char* path, SDL_Renderer * renderer)
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
			texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
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
	bool loadFromRenderedText(SDL_Renderer * renderer, char* textureText, SDL_Color textColor)
	{
		bool success = true;
		free();
		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText, textColor);
		if (textSurface != NULL)
		{
			//Create texture from surface pixels
			texture = SDL_CreateTextureFromSurface(renderer, textSurface);
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
	void render(SDL_Renderer * renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		SDL_Rect quad = { x, y, width, height };
		if (clip != NULL)
		{
			quad.w = clip->w;
			quad.h = clip->h;
		}
		SDL_RenderCopyEx(renderer, texture, clip, &quad, angle, center, flip);
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
	}
	~Window()
	{
	}
	void init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL init error");
			exit(-1);
		}
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		window = SDL_CreateWindow("Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Cannot create window. Error: %s\n", SDL_GetError());
			exit(-1);
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
			printf("Cannot create renderer. Error: %s\n", SDL_GetError());
			exit(-1);
		}
		//Init PNG loading
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("Cannot init PNG loading. Error: %s\n", IMG_GetError());
			exit(-1);
		}
		loop();
	}
private:
	void loop()
	{
		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
					quit = true;
			}
		}
	}
private:
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event e;
	bool quit = false;
};