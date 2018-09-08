#pragma once
#include "Tile.h"
#include "Object.h"

#define TICKRATE 32
#define MS_PER_TICK (1000/TICKRATE)

class Map
{
public:
	Map() noexcept : tilesFileWidth{ 0 }, totalTileSetTiles { 0 }, width{ 0 }, height{ 0 }, totalTiles{ 0 }, tileSet{ NULL }, tileInfo{ NULL }, tilesPlace{ NULL }
	{}

	// -Call it without extension
	//   +The files are needed to run map:
	//     .txt containing all tiles counting from left side in PNG,
	//     .map containing map data
	//     .png containing tiles
	void setMap(const int _width, const int _height, const char * path);

	SDL_Rect* getTilesPlace() const noexcept;

	bool collides(SDL_Rect& box) const;

private:
	void loadTiles(const char * path);

	void createTiles(const char * path);

	void loadTilesPlace() const noexcept;

	void free();

	void loadObjects(const char * path);

private:
	int tilesFileWidth;
	int totalTileSetTiles;
	TileInfo* tileInfo;

public:
	int width;
	int height;
	int totalTiles;

	Tile* tileSet;
	SDL_Rect* tilesPlace;

	int objCount{};
	Object * objects{};
};
