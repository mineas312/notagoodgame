#include "stdafx.h"

#include "Tile.h"

Tile::Tile() noexcept : box{ 0,0,TILE_WIDTH, TILE_HEIGHT }, type{ 0 }
{}

void Tile::setTile(const int x, const int y, const int _type, TileInfo & info)
{
	box.x = x;
	box.y = y;
	type = _type;
	tileInfo = info;
}

void Tile::render(SDL_Renderer * renderer, SDL_Rect & camera, SDL_Rect & clip)
{
	if (checkCollision(camera, box))
		mptr->tilesTexture.render(renderer, box.x - camera.x, box.y - camera.y, &clip);
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
