#pragma once
#include "Object.h"

class Media
{
public:
	void loadMedia();

	Object tilesTexture;
	Object charTexture;
};

extern Media* mptr;
