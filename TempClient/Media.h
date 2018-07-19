#pragma once
#include "Texture.h"

class Media
{
public:
	Media() noexcept;

	void loadMedia();

public:
	Texture charTexture;
	Texture * mapTilesTexture;
};

extern Media* mptr;
