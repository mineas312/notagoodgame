#pragma once

#define TILE_WIDTH 20
#define TILE_HEIGHT 20

#include "Texture.h"
#include "Media.h"

struct TileInfo
{
	std::string name;
	bool canMoveThrough;
};

class Tile
{
public:
	void setTile(const int x, const int y, const int _type, TileInfo& info)
	{
		box.x = x;
		box.y = y;
		box.w = TILE_WIDTH;
		box.h = TILE_HEIGHT;
		type = _type;
		tileInfo = info;
	}

	void render(SDL_Renderer* renderer, SDL_Rect& camera, SDL_Rect& clip)
	{
		if (checkCollision(camera, box))
			mptr->tilesTexture.render(renderer, box.x - camera.x, box.y - camera.y, &clip);
	}

	void setName(TileInfo& _tileInfo)
	{
		tileInfo = _tileInfo;
	}

private:
	bool checkCollision(SDL_Rect& a, SDL_Rect& b)
	{
		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;

		leftA = a.x;
		rightA = a.x + a.w;
		leftB = b.x;
		rightB = b.x + b.w;
		
		topA = a.y;
		bottomA = a.y + a.h;
		topB = b.y;
		bottomB = b.y + b.h;

		if (bottomA <= topB)
			return false;

		if (topA >= bottomB)
			return false;

		if (rightA <= leftB)
			return false;

		if (leftA >= rightB)
			return false;

		return true;
	}

public:
	TileInfo tileInfo;
	SDL_Rect box;
	int type;
};
