#pragma once
#include <SDL_image.h>
#include <cstdio>

#define KEY_COLOR_R 0
#define KEY_COLOR_G 0xFF
#define KEY_COLOR_B 0xFF

class Texture
{
public:
	Texture() : texture{ NULL }, width{ 0 }, height{ 0 }
	{}

	bool loadTexture(const char* path, SDL_Renderer* renderer)
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
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, KEY_COLOR_R, KEY_COLOR_G, KEY_COLOR_B));
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

	void free()
	{
		if (texture != NULL)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
			width = 0;
			height = 0;
		}
	}

	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		SDL_Rect quad = { x, y, width, height };
		if (clip != NULL)
		{
			quad.w = clip->w;
			quad.h = clip->h;
		}
		SDL_RenderCopyEx(renderer, texture, clip, &quad, angle, center, flip);
	}

	int getWidth()
	{
		return width;
	}

	int getHeight()
	{
		return height;
	}

private:
	SDL_Texture* texture;
	int width;
	int height;
};
