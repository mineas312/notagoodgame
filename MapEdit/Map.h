#pragma once
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <fstream>
#include <vector>

#include "Window.h"
#include "Object.h"
#include "Texture.h"

extern Texture g_TileSetTiles;
extern Texture* g_ObjectTextures;

struct TileInfo
{
	bool canMoveThrough = false;
	std::string name;
};

class Tile
{
public:
	Tile() = default;

	Tile(int x, int y, int _type, TileInfo& _tileInfo);

	void setTile(int x, int y, int _type, TileInfo& _tileInfo);

	void render(SDL_Rect* clip = NULL);

public:
	SDL_Rect box;
	int type;
	TileInfo tileInfo;
};

class Map
{
public:
	Map() = default;

	void setMap(std::string path, int _width, int _height);

private:
	void loadTiles(std::string path);

	void createTiles(const char* path);

	void loadTilesPlace() noexcept;

	void free();

	void loadObjects(const char* path);

	void createNewMap(const char* path);

public:
	int width = 0;
	int height = 0;
	int totalTiles = 0;
	int totalTileSetTiles = 0;
	SDL_Rect* tilesPlace = nullptr;
	TileInfo* tileInfo = nullptr;
	Tile* tileSet = nullptr;
	int objCount = 0;
	std::vector<Object> objects;
};