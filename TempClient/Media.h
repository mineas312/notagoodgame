#pragma once
#include "Texture.h"

class Media
{
public:
	Media()
	{

	}
	~Media()
	{

	}
	void loadMedia(SDL_Renderer * renderer)
	{
		tilesTexture.loadTexture("res/lazy.png", renderer);
		charTexture.loadTexture("res/char.png", renderer);
	}
public:
	Texture tilesTexture;
	Texture charTexture;
};

Media * mptr;