#pragma once
#include "Texture.h"

class Media
{
public:
	void loadMedia(SDL_Renderer* renderer);

	Texture tilesTexture;
	Texture charTexture;
};

extern Media* mptr;
