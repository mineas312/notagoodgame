#include "Texture.h"
#include <cstdio>
#include <SDL_image.h>

TTF_Font * font;
SDL_Renderer * g_renderer;

bool check_collision(const SDL_Rect& a, const SDL_Rect& b)
{
	return !(a.y + a.h <= b.y || (b.y + b.h) <= a.y || (a.x + a.w) <= b.x || (b.x + b.w) <= a.x);
}

bool Texture::load_texture(const char * path)
{
	bool success = true;
	free();

	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == nullptr)
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

bool Texture::loadFromRenderedText(char* textureText, SDL_Color textColor)
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

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) const
{
	SDL_Rect quad = { x, y, width, height };
	if (clip != NULL)
	{
		quad.w = clip->w;
		quad.h = clip->h;
	}

	SDL_RenderCopyEx(g_renderer, texture, clip, &quad, angle, center, flip);
}
