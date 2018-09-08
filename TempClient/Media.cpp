#include "stdafx.h"

#include "Media.h"

Media::Media() noexcept : mapTilesTexture{ NULL }, mapObjTextures{ NULL }
{
}

void Media::loadMedia()
{
#ifdef USE_FORMAT_PNG
	charTexture.set_texture("res/character.png");
#else
	charTexture.setTexture("res/character.KTX");
#endif
}

Media* mptr;