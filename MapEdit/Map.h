#pragma once
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <fstream>

#include "Window.h"

Texture g_TileSetTiles;

struct TileInfo
{
	bool canMoveThrough = false;
	std::string name;
};

class Tile
{
public:
	Tile()
	{

	}
	Tile(int x, int y, int _type, TileInfo & _tileInfo)
	{
		//Get the offsets
		box.x = x;
		box.y = y;

		//Set the collision box
		box.w = TILE_WIDTH;
		box.h = TILE_HEIGHT;

		//Get the tile type
		type = _type;
		tileInfo = _tileInfo;
	}
	void setTile(int x, int y, int _type, TileInfo & _tileInfo)
	{
		//Get the offsets
		box.x = x;
		box.y = y;

		//Set the collision box
		box.w = TILE_WIDTH;
		box.h = TILE_HEIGHT;

		//Get the tile type
		type = _type;
		tileInfo = _tileInfo;
	}
	bool check_collision(SDL_Rect &a, SDL_Rect &b)
	{
		return (a.y + a.h) <= b.y || (b.y + b.h) <= a.y || (a.x + a.w) <= b.x || (b.x + b.w) <= a.x ? false : true;
	}
	void render(SDL_Rect* clip = NULL)
	{
		if(clip == NULL)
			g_TileSetTiles.render(box.x, box.y);
		else
			g_TileSetTiles.render(box.x, box.y, clip);
	}
public:
	SDL_Rect box;
	int type;
	TileInfo tileInfo;
};

class Map
{
public:
	Map()
	{
		width = 0;
		height = 0;
		totalTiles = 0;
		totalTileSetTiles = 0;
	}
	~Map()
	{

	}
	void setMap(std::string path, int _width, int _height)
	{
		// TODO free

		width = _width;
		height = _height;
		totalTiles = width / TILE_WIDTH * height / TILE_HEIGHT;

		loadTiles(path);
	}
	void loadTiles(std::string path)
	{
		std::stringstream ctxt;
		ctxt << path << ".txt";

		std::ifstream txt(ctxt.str());
		if (!txt.is_open())
		{
			printf("Unable to load tile's info!\n");
			fprintf(stderr, "Unable to load tile's info!\n");
			exit(-4);
		}
		txt >> totalTileSetTiles;

		tilesPlace = new SDL_Rect[static_cast<uint64_t>(totalTileSetTiles)];
		tileInfo = new TileInfo[static_cast<uint64_t>(totalTileSetTiles)];
		std::string tmp;

		int i = 0;
		while (std::getline(txt, tmp))
		{
			if (tmp == "")
				continue;

			if (tmp == "0" || tmp == "1")
			{
				tileInfo[i].canMoveThrough = std::atoi(tmp.c_str());
				i++;
			}
			else
				tileInfo[i].name = tmp;
		}
		txt.close();

		std::stringstream ctile;
		ctile << path << ".png";

		g_TileSetTiles.loadTexture(ctile.str().c_str());

		createTiles(path.c_str());
	}
	void createTiles(const char * path)
	{
		tileSet = new Tile[static_cast<uint64_t>(totalTiles)];
		int x = 0, y = 0;

		std::stringstream cmap;

		cmap << path << ".map";

		std::ifstream map(cmap.str());
		if (!map.is_open())
		{
			std::printf("Unable to load map file!\n");
			std::fprintf(stderr, "Unable to load map file!\n");
			std::exit(-4);
		}
		else
		{
			loadTilesPlace();

			for (int i = 0; i < totalTiles; i++)
			{
				int tileType = -1;
				map >> tileType;

				if (map.fail())
				{
					printf("Error loading map: Unexpected end of file!\n");
					fprintf(stderr, "Error loading map: Unexpected end of file!\n");
					break;
				}

				if (tileType >= 0 && tileType <= totalTileSetTiles)
				{
					tileSet[i].setTile(x, y, tileType, tileInfo[tileType]);
				}
				else
				{
					printf("Error loading map: Invalid tile type at %d!\n", i);
					fprintf(stderr, "Error loading map: Invalid tile type at %d!\n", i);
					break;
				}

				x += TILE_WIDTH;
				if (x >= width)
				{
					x = 0;
					y += TILE_HEIGHT;
				}
			}
		}
		map.close();
	}
	void loadTilesPlace() noexcept
	{
		for (int i = 0; i < totalTileSetTiles; i++)
		{
			int widthT = TILE_WIDTH * i;
			int heightT = 0;
			while (widthT >= g_TileSetTiles.width)
			{
				widthT -= g_TileSetTiles.width;
				heightT += TILE_HEIGHT;
			}

			tilesPlace[i].x = widthT;
			tilesPlace[i].y = heightT;
			tilesPlace[i].w = TILE_WIDTH;
			tilesPlace[i].h = TILE_HEIGHT;
		}
	}
private:
	int width;
	int height;
	int totalTiles;
	int totalTileSetTiles;
	SDL_Rect * tilesPlace;
	TileInfo * tileInfo;
	Tile * tileSet;
};