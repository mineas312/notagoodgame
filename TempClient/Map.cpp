#include "stdafx.h"

#include "Map.h"

// -Call it without extension
//   +The files are needed to run map:
//     .txt containing all tiles counting from left side in PNG,
//     .map containing map data
//     .png containing tiles

void Map::setMap(const int _width, const int _height, const char * path)
{
	free();

	width = _width;
	height = _height;
	totalTiles = width / TILE_WIDTH * height / TILE_HEIGHT;

	loadTiles(path);
}

SDL_Rect * Map::getTilesPlace() noexcept
{
	return tilesPlace;
}

bool Map::collides(SDL_Rect & box)
{
	// TODO: Optimize loop
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

void Map::loadTiles(const char * path)
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

	if (mptr->mapTilesTexture != NULL)
	{
		delete[] mptr->mapTilesTexture;
		mptr->mapTilesTexture = NULL;
	}

	std::stringstream ctile;
	ctile << path << ".png";

	Texture * tiles = new Texture[static_cast<uint64_t>(totalTileSetTiles)];

	// Dimension check
	tiles[0].setTexture(ctile.str().c_str());
	tilesFileWidth = tiles[0].texWidth;

	createTiles(path);

	for (int ii = 0; ii < totalTileSetTiles; ii++)
	{
		tiles[ii].setTexture(ctile.str().c_str(), &tilesPlace[ii]);
	}

	mptr->mapTilesTexture = tiles;
}

void Map::createTiles(const char * path)
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

void Map::loadTilesPlace() noexcept
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

void Map::free()
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

bool Map::checkCollision(const SDL_Rect & a, const SDL_Rect & b) noexcept
{
	return (a.y + a.h) <= b.y || (b.y + b.h) <= a.y || (a.x + a.w) <= b.x || (b.x + b.w) <= a.x ? false : true;

	/*int leftA = 0, leftB = 0;
	int rightA = 0, rightB = 0;
	int topA = 0, topB = 0;
	int bottomA = 0, bottomB = 0;

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
	if (bottomB <= topA)
		return false;
	if (rightA <= leftB)
		return false;
	if (rightB <= leftA)
		return false;

	return true;*/
}