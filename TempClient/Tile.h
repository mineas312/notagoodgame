#pragma once
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#include "Texture.h"
#include "Media.h"

struct TileInfo
{
	bool canMoveThrough = false;
	std::string name;
};

class Tile
{
public:
	Tile() noexcept;

	void setTile(const int x, const int y, const int _type, TileInfo& info);

	void render();

	void setName(TileInfo& _tileInfo);

private:
	bool checkCollision(SDL_Rect& a, SDL_Rect& b);

public:
	int type;
	TileInfo tileInfo;
	SDL_Rect box;
};
