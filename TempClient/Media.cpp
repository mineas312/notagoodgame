#include "stdafx.h"

#include "Media.h"

Media::Media() noexcept : mapTilesTexture{ NULL }
{
}

void Media::loadMedia()
{
	charTexture.setTexture("res/character.png");
}

Media* mptr;
