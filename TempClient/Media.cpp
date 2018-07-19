#include "stdafx.h"

#include "Media.h"

Media::Media() noexcept : mapTilesTexture{ NULL }
{
}

void Media::loadMedia()
{
	charTexture.setTexture("res/char.png");
}

Media* mptr;
