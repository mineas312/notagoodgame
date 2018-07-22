#pragma once
#include "Tile.h"

class Map
{
public:
	Map() noexcept : tilesFileWidth{ 0 }, totalTileSetTiles { 0 }, width{ 0 }, height{ 0 }, totalTiles{ 0 }, tileSet{ NULL }, tilesPlace{ NULL }, tileInfo{ NULL }
	{}

	// -Call it without extension
	//   +The files are needed to run map:
	//     .txt containing all tiles counting from left side in PNG,
	//     .map containing map data
	//     .png containing tiles
	void setMap(const int _width, const int _height, const char* path);

	SDL_Rect* getTilesPlace() noexcept;

	bool collides(SDL_Rect& box);

private:
	void loadTiles(const char* path);

	void createTiles(const char* path);

	void loadTilesPlace() noexcept;

	void free();

private:
	int tilesFileWidth;
	int totalTileSetTiles;
	TileInfo* tileInfo;

public:
	int width;
	int height;
	int totalTiles;
	GLuint vao;

	Tile* tileSet;
	SDL_Rect* tilesPlace;
};
