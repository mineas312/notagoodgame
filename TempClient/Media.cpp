#include "stdafx.h"

#include "Media.h"

Media::Media() noexcept : mapTilesTexture{ NULL }
{
}

void Media::loadMedia()
{
#ifdef USE_FORMAT_PNG
	charTexture.setTexture("res/character.png");
#else
	charTexture.setTexture("res/character.KTX");
#endif
}

Media* mptr;
