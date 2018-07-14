#pragma once
#include "Texture.h"

class Media
{
public:
	void loadMedia(SDL_Renderer* renderer)
	{
		tilesTexture.loadTexture("res/lazy.png", renderer);
		charTexture.loadTexture("res/char.png", renderer);
	}

	Texture tilesTexture;
	Texture charTexture;
};

Media* mptr;
