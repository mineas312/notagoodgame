#include "stdafx.h"

#include "Media.h"

void Media::loadMedia(SDL_Renderer * renderer)
{
	tilesTexture.loadTexture("res/lazy.png", renderer);
	charTexture.loadTexture("res/char.png", renderer);
}

Media* mptr;
