#include "stdafx.h"
#include "Tile.h"
#include "Common.h"

Tile::Tile() noexcept : box{ 0, 0, TILE_WIDTH, TILE_HEIGHT }, type{ 0 }
{}

void Tile::set_tile(const int x, const int y, const int _type, TileInfo & info)
{
	box.x = x;
	box.y = y;
	type = _type;
	tile_info = info;
}