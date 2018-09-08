#pragma once
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#include "Texture.h"

struct TileInfo
{
	bool canMoveThrough = false;
	std::string name;
};

class Tile
{
public:
	Tile() noexcept;

	void set_tile(int x, int y, int _type, TileInfo& info);

public:
	int type;
	TileInfo tile_info;
	SDL_Rect box;
};
