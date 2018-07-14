#include "stdafx.h"

#include "Texture.h"

bool Texture::loadTexture(const char * path, SDL_Renderer * renderer)
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

void Texture::free()
{
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::render(SDL_Renderer * renderer, int x, int y, SDL_Rect * clip, double angle, SDL_Point * center, SDL_RendererFlip flip)
{
	SDL_Rect quad = { x, y, width, height };
	if (clip != NULL)
	{
		quad.w = clip->w;
		quad.h = clip->h;
	}
	SDL_RenderCopyEx(renderer, texture, clip, &quad, angle, center, flip);
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}
