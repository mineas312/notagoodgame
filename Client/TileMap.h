#pragma once

#define TILE_WIDTH 80
#define TILE_HEIGHT 80

#include "Texture.h"
#include <fstream>

enum TileEnum
{
	TILE_RED,
	TILE_GREEN,
	TILE_BLUE,
	TILE_CENTER,
	TILE_TOP,
	TILE_TOPRIGHT,
	TILE_RIGHT,
	TILE_BOTTOMRIGHT,
	TILE_BOTTOM,
	TILE_BOTTOMLEFT,
	TILE_LEFT,
	TILE_TOPLEFT,
	TOTAL_TILES,
	NO_TILE
};

Texture tilesTexutre;
SDL_Rect gTiles[TOTAL_TILES];

class Tile
{
public:
	Tile()
	{}
	Tile(int x, int y, TileEnum tileType)
	{
		box.x = x;
		box.y = y;
		box.w = TILE_WIDTH;
		box.h = TILE_HEIGHT;
		type = tileType;
	}
	void setTile(int x, int y, TileEnum tileType)
	{
		box.x = x;
		box.y = y;
		box.w = TILE_WIDTH;
		box.h = TILE_HEIGHT;
		type = tileType;
	}
	void render(SDL_Renderer * renderer, SDL_Rect& camera)
	{
		if (checkCollision(camera, box))
		{
			tilesTexutre.render(renderer, box.x - camera.x, box.y - camera.y, &gTiles[type]);
		}
	}
	TileEnum getType()
	{
		return type;
	}
	SDL_Rect getBox()
	{
		return box;
	}
private:
	bool checkCollision(SDL_Rect a, SDL_Rect b)
	{
		//The sides of the rectangles
		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;

		//Calculate the sides of rect A
		leftA = a.x;
		rightA = a.x + a.w;
		topA = a.y;
		bottomA = a.y + a.h;

		//Calculate the sides of rect B
		leftB = b.x;
		rightB = b.x + b.w;
		topB = b.y;
		bottomB = b.y + b.h;

		//If any of the sides from A are outside of B
		if (bottomA <= topB)
		{
			return false;
		}

		if (topA >= bottomB)
		{
			return false;
		}

		if (rightA <= leftB)
		{
			return false;
		}

		if (leftA >= rightB)
		{
			return false;
		}

		//If none of the sides from A are outside B
		return true;
	}
private:
	SDL_Rect box;
	TileEnum type;
};

enum MapEnum
{
	DEBUG,
	TOTAL_MAPS
};

class Map
{
public:
	Map()
	{
		MAP_WIDTH = 0;
		MAP_HEIGHT = 0;
		MAP_TILES = 0;
	}
	~Map()
	{
		delete[] tileSet;
	}
	void setMap(int width, int height, const char* path)
	{
		MAP_WIDTH = width;
		MAP_HEIGHT = height;
		MAP_TILES = MAP_WIDTH / TILE_WIDTH * MAP_HEIGHT / TILE_HEIGHT;
		createTiles(path);
	}
	bool touchesWall(SDL_Rect box)
	{
		for (int i = 0; i < MAP_TILES; i++)
		{
			int type = (int)tileSet[i].getType();
			if ((type >= (int)TILE_CENTER) && (type <= (int)TILE_TOPLEFT))
			{
				if (checkCollision(box, tileSet[i].getBox()))
					return true;
			}
		}
		return false;
	}
private:
	bool checkCollision(SDL_Rect a, SDL_Rect b)
	{
		//The sides of the rectangles
		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;

		//Calculate the sides of rect A
		leftA = a.x;
		rightA = a.x + a.w;
		topA = a.y;
		bottomA = a.y + a.h;

		//Calculate the sides of rect B
		leftB = b.x;
		rightB = b.x + b.w;
		topB = b.y;
		bottomB = b.y + b.h;

		//If any of the sides from A are outside of B
		if (bottomA <= topB)
		{
			return false;
		}

		if (topA >= bottomB)
		{
			return false;
		}

		if (rightA <= leftB)
		{
			return false;
		}

		if (leftA >= rightB)
		{
			return false;
		}

		//If none of the sides from A are outside B
		return true;
	}
	bool createTiles(const char* path)
	{
		bool success = true;
		tileSet = new Tile[MAP_TILES];
		int x = 0, y = 0;
		std::ifstream map(path);
		if (!map.is_open())
		{
			printf("Unable to load map file!\n");
			success = false;
		}
		else
		{
			for (int i = 0; i < MAP_TILES; i++)
			{
				int tileType = -1;
				map >> tileType;
				if (map.fail())
				{
					printf("Error loading map: Unexpected end of file!\n");
					success = false;
					break;
				}
				if ((tileType >= 0) && (tileType < TOTAL_TILES))
					tileSet[i].setTile(x, y, TileEnum(tileType));
				else
				{
					printf("Error loading map: Invalit tile type at %d!\n", i);
					success = false;
					break;
				}
				x += TILE_WIDTH;
				if (x >= MAP_WIDTH)
				{
					x = 0;
					y += TILE_HEIGHT;
				}
			}
			if (success)
			{
				gTiles[TILE_RED].x = 0;
				gTiles[TILE_RED].y = 0;
				gTiles[TILE_RED].w = TILE_WIDTH;
				gTiles[TILE_RED].h = TILE_HEIGHT;

				gTiles[TILE_GREEN].x = 0;
				gTiles[TILE_GREEN].y = 80;
				gTiles[TILE_GREEN].w = TILE_WIDTH;
				gTiles[TILE_GREEN].h = TILE_HEIGHT;

				gTiles[TILE_BLUE].x = 0;
				gTiles[TILE_BLUE].y = 160;
				gTiles[TILE_BLUE].w = TILE_WIDTH;
				gTiles[TILE_BLUE].h = TILE_HEIGHT;

				gTiles[TILE_TOPLEFT].x = 80;
				gTiles[TILE_TOPLEFT].y = 0;
				gTiles[TILE_TOPLEFT].w = TILE_WIDTH;
				gTiles[TILE_TOPLEFT].h = TILE_HEIGHT;

				gTiles[TILE_LEFT].x = 80;
				gTiles[TILE_LEFT].y = 80;
				gTiles[TILE_LEFT].w = TILE_WIDTH;
				gTiles[TILE_LEFT].h = TILE_HEIGHT;

				gTiles[TILE_BOTTOMLEFT].x = 80;
				gTiles[TILE_BOTTOMLEFT].y = 160;
				gTiles[TILE_BOTTOMLEFT].w = TILE_WIDTH;
				gTiles[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

				gTiles[TILE_TOP].x = 160;
				gTiles[TILE_TOP].y = 0;
				gTiles[TILE_TOP].w = TILE_WIDTH;
				gTiles[TILE_TOP].h = TILE_HEIGHT;

				gTiles[TILE_CENTER].x = 160;
				gTiles[TILE_CENTER].y = 80;
				gTiles[TILE_CENTER].w = TILE_WIDTH;
				gTiles[TILE_CENTER].h = TILE_HEIGHT;

				gTiles[TILE_BOTTOM].x = 160;
				gTiles[TILE_BOTTOM].y = 160;
				gTiles[TILE_BOTTOM].w = TILE_WIDTH;
				gTiles[TILE_BOTTOM].h = TILE_HEIGHT;

				gTiles[TILE_TOPRIGHT].x = 240;
				gTiles[TILE_TOPRIGHT].y = 0;
				gTiles[TILE_TOPRIGHT].w = TILE_WIDTH;
				gTiles[TILE_TOPRIGHT].h = TILE_HEIGHT;

				gTiles[TILE_RIGHT].x = 240;
				gTiles[TILE_RIGHT].y = 80;
				gTiles[TILE_RIGHT].w = TILE_WIDTH;
				gTiles[TILE_RIGHT].h = TILE_HEIGHT;

				gTiles[TILE_BOTTOMRIGHT].x = 240;
				gTiles[TILE_BOTTOMRIGHT].y = 160;
				gTiles[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
				gTiles[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
			}
		}
		map.close();
		return success;
	}
public:
	int MAP_WIDTH;
	int MAP_HEIGHT;
	int MAP_TILES;
	Tile * tileSet;
};