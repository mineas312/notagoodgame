#include "stdafx.h"
#include "Camera.h"
#include "Tile.h"
#include "Common.h"
Tile::Tile() noexcept : box{ 0, 0, TILE_WIDTH, TILE_HEIGHT }, type{ 0 }
{}

void Tile::setTile(const int x, const int y, const int _type, TileInfo & info)
{
	box.x = x;
	box.y = y;
	type = _type;
	tileInfo = info;
}

void Tile::render()
{
	if (checkCollision(camptr->camRect, box))
		mptr->mapTilesTexture[type].render(box.x, box.y);
}

void Tile::setName(TileInfo & _tileInfo)
{
	tileInfo = _tileInfo;
}
