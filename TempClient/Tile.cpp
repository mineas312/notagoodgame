#include "stdafx.h"

#include "Tile.h"

Tile::Tile() noexcept : box{ 0,0,TILE_WIDTH, TILE_HEIGHT }, type{ 0 }
{}

void Tile::setTile(const int x, const int y, const int _type, TileInfo & info, SDL_Rect & _clip)
{
	box.x = x;
	box.y = y;
	type = _type;
	tileInfo = info;
	clip = _clip;
}

void Tile::render(SDL_Rect & camera)
{
	if (checkCollision(camera, box))
		mptr->tilesTexture.render(box.x - camera.x, box.y - camera.y);
}

void Tile::setName(TileInfo & _tileInfo)
{
	tileInfo = _tileInfo;
}

bool Tile::checkCollision(SDL_Rect & a, SDL_Rect & b)
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
