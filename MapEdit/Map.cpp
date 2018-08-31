#include "Map.h"

Texture g_TileSetTiles;
Texture * g_ObjectTextures;

Tile::Tile(int x, int y, int _type, TileInfo& _tileInfo)
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

void Tile::setTile(int x, int y, int _type, TileInfo& _tileInfo)
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

void Tile::render(SDL_Rect* clip)
{
	if (clip == NULL)
		g_TileSetTiles.render(box.x - wptr->camera.x, box.y - wptr->camera.y);
	else
		g_TileSetTiles.render(box.x - wptr->camera.x, box.y - wptr->camera.y, clip);
}

void Map::setMap(std::string path, int _width, int _height)
{
	free();

	width = _width;
	height = _height;
	totalTiles = width / TILE_WIDTH * height / TILE_HEIGHT;

	loadTiles(path);
	loadObjects(path.c_str());
}

void Map::loadTiles(std::string path)
{
	std::stringstream ctxt;
	ctxt << path << "/tilesetInfo.txt";

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
	ctile << path << "/tileset.png";

	g_TileSetTiles.loadTexture(ctile.str().c_str());

	createTiles(path.c_str());
}

void Map::createTiles(const char* path)
{
	tileSet = new Tile[static_cast<uint64_t>(totalTiles)];
	int x = 0, y = 0;

	std::stringstream cmap;

	cmap << path << "/mapFile.map";

	std::ifstream map(cmap.str());
	if (!map.is_open())
	{
		std::printf("Unable to load map file!\n");
		std::fprintf(stderr, "Unable to load map file!\n");
		createNewMap(path);
		exit(-4);
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
				createNewMap(path);
				exit(-4);
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
				createNewMap(path);
				exit(-4);
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

void Map::free()
{
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

	if (tileSet != NULL)
	{
		delete[] tileSet;
		tileSet = NULL;
	}
}

void Map::loadObjects(const char* path)
{
	std::stringstream ctxt;
	ctxt << path << "/objects.txt";

	std::ifstream txt(ctxt.str());
	if (!txt.is_open())
	{
		printf("Unable to load tile's info!\n");
		fprintf(stderr, "Unable to load tile's info!\n");
		exit(-4);
	}

	int count;
	txt >> count;
	g_ObjectTextures = new Texture[count];
	for (int i = 0; i < count; i++)
	{
		std::stringstream sobj;
		sobj << path << "/obj/obj" << i << ".png";
		g_ObjectTextures[i].loadTexture(sobj.str().c_str());
	}

	txt >> objCount;
	for (int i = 0; i < objCount; i++)
	{
		int owidth, oheight, ox, oy, oid;
		bool cmt;

		txt >> owidth;
		txt >> oheight;
		txt >> ox;
		txt >> oy;
		txt >> cmt;
		txt >> oid;

		Object obj;
		obj.set(owidth, oheight, ox, oy, cmt, oid);
		objects.push_back(obj);
	}
	objCount = count;
	txt.close();
}

void Map::createNewMap(const char* path)
{
	std::stringstream ss;
	ss << path << "/mapFile.map";
	std::ofstream file(ss.str());
	for (int i = 0; i < totalTiles; i++)
		file << "0 ";
	file.close();
	printf("The corrupted map has been replaced by the new one!\n");
	system("pause");
}
