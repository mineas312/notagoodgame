#pragma once
#define TILE_WIDTH 20
#define TILE_HEIGHT 20

#include "Texture.h"
#include "Media.h"

struct TileInfo
{
	std::string name;
	bool canMoveThrough = false;
};

class Tile
{
public:
	void setTile(const int x, const int y, const int _type, TileInfo& info);

	void render(SDL_Renderer* renderer, SDL_Rect& camera, SDL_Rect& clip);

	void setName(TileInfo& _tileInfo);

private:
	bool checkCollision(SDL_Rect& a, SDL_Rect& b);

public:
	TileInfo tileInfo;
	SDL_Rect box;
	int type = 0;
};
