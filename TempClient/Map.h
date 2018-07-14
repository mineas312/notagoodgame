#pragma once
#include <fstream>
#include "Tile.h"
#include <string>

class Map
{
public:
	Map()
	{
		width = 0;
		height = 0;
		totalTiles = 0;
		tileSet = NULL;
		tilesPlace = NULL;
		tileInfo = NULL;
	}
	~Map()
	{
	}
	// -Call it without extension
	//   +The files are needed to run map:
	//     .txt containing all tiles counting from left side in PNG,
	//     .map containing map data
	//     .png containing tiles
	void setMap(int width, int height, const char * path, SDL_Renderer * renderer)
	{
		free();

		this->width = width;
		this->height = height;
		totalTiles = this->width / TILE_WIDTH * this->height / TILE_HEIGHT;
		loadTiles(path, renderer);
	}
	SDL_Rect * getTilesPlace()
	{
		return tilesPlace;
	}
	bool collides(SDL_Rect box)
	{
		// TODO Optimize loop
		for (int i = 0; i < totalTiles; i++)
		{
			if (!tileSet[i].tileInfo.canMoveThrough)
			{
				if (checkCollision(box, tileSet[i].box))
					return true;
			}
		}
		return false;
	}
private:
	void loadTiles(const char * path, SDL_Renderer * renderer)
	{
		char * ctxt = new char[strlen(path)+5];
		strcpy(ctxt, path);
		strcat(ctxt, ".txt");
		std::ifstream txt(ctxt);
		if (!txt.is_open())
		{
			printf("Unable to load tile's info!\n");
			fprintf(stderr, "Unable to load tile's info!\n");
			exit(-4);
		}
		txt >> totalTileSetTiles;

		tilesPlace = new SDL_Rect[totalTileSetTiles];

		tileInfo = new TileInfo[totalTileSetTiles];
		std::string tmp;
		int i = 0, d = 0;
		while (std::getline(txt, tmp))
		{
			if (!tmp.compare(""))
				continue;
			if (!tmp.compare("0") || !tmp.compare("1"))
			{
				tileInfo[i].canMoveThrough = atoi(tmp.c_str());
				i++;
			}
			else
				tileInfo[i].name = tmp;
		}
		txt.close();

		char * ctile = new char[strlen(path)+5];
		strcpy(ctile, path);
		strcat(ctile, ".png");
		Texture tiles;
		tiles.loadTexture(ctile, renderer);
		SDL_Surface * surface = IMG_Load(ctile);
		int w = surface->w, h = surface->h;
		tilesFileWidth = w;

		delete[] ctxt;
		delete[] ctile;

		createTiles(path);
	}
	void createTiles(const char * path)
	{
		tileSet = new Tile[totalTiles];
		int x = 0, y = 0;
		char * cmap = new char[strlen(path)+5];
		strcpy(cmap, path);
		strcat(cmap, ".map");
		std::ifstream map(cmap);
		if (!map.is_open())
		{
			printf("Unable to load map file!\n");
			fprintf(stderr, "Unable to load map file!\n");
			exit(-4);
		}
		else
		{
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
		delete cmap;
		loadTilesPlace();
	}
	void loadTilesPlace()
	{
		for (int i = 0; i < totalTileSetTiles; i++)
		{
			int widthT = TILE_WIDTH * i;
			int heightT = 0;
			while (widthT >= tilesFileWidth)
			{
				widthT -= tilesFileWidth;
				heightT += TILE_HEIGHT;
			}
			tilesPlace[i].x = widthT;
			tilesPlace[i].y = heightT;
			tilesPlace[i].w = TILE_WIDTH;
			tilesPlace[i].h = TILE_HEIGHT;
		}
	}
	void free()
	{
		if (tileSet != NULL)
		{
			delete[] tileSet;
			tileSet = NULL;
		}
		if (tilesPlace != NULL)
		{
			delete[] tilesPlace;
			tilesPlace = NULL;
		}
		if (tileInfo != NULL)
		{
			delete[] tileInfo;
			tileInfo = NULL;
		}
	}
	bool checkCollision(SDL_Rect a, SDL_Rect b)
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
private:
	int tilesFileWidth;
	TileInfo * tileInfo;
	int totalTileSetTiles;
public:
	Tile * tileSet;
	SDL_Rect * tilesPlace;
	int width;
	int height;
	int totalTiles;
};