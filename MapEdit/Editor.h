#pragma once
#include "Map.h"

class Editor
{
public:
	Editor(int screen_width, int screen_height)
	{
		wptr = new Window(screen_width, screen_height);
		quit = false;
		currentType = 0;
	}
	// -Call witout extension; needs .txt .map .png
	void init(std::string map, int mapWidth, int mapHeight)
	{
		if (!wptr->init())
		{
			exit(1);
		}
		m.setMap(map, mapWidth, mapHeight);
		mapFile = map;
	}
	void loop()
	{
		Uint64 startClock = SDL_GetPerformanceCounter();
		Uint64 lastClock = 0;

		while (!quit)
		{
			lastClock = startClock;
			startClock = SDL_GetPerformanceCounter();

			currentFPS = 1000.0 / ((startClock - lastClock) * 1000.0 / static_cast<double>(SDL_GetPerformanceFrequency()));

			while (SDL_PollEvent(&wptr->e) != 0)
			{
				if (wptr->e.type == SDL_QUIT)
					quit = true;
				if (wptr->e.type == SDL_MOUSEBUTTONDOWN)
				{
					if (wptr->e.button.button == SDL_BUTTON_LEFT)
						putTile();
				}
				if (wptr->e.type == SDL_MOUSEWHEEL)
				{
					if (wptr->e.wheel.y > 0) // scroll up
					{
						if (currentType + 1 < m.totalTileSetTiles)
						{
							currentType++;
							std::stringstream title;
							title << "Level Designer. Current Tile ID: " << currentType;
							SDL_SetWindowTitle(wptr->window, title.str().c_str());
						}
					}
					else if (wptr->e.wheel.y < 0) // scroll down
					{
						if (currentType >= 1)
						{
							currentType--;
							std::stringstream title;
							title << "Level Designer. Current Tile ID: " << currentType;
							SDL_SetWindowTitle(wptr->window, title.str().c_str());
						}
					}
					
				}
			}

			render();
		}
		saveMap();
	}
	void render()
	{

		wptr->set_camera(m.width, m.height, currentFPS);

		for (int i = 0; i < m.totalTiles; i++)
		{
			if (check_collision(wptr->camera, m.tileSet[i].box))
				m.tileSet[i].render(&m.tilesPlace[m.tileSet[i].type]);
		}

		SDL_RenderPresent(g_renderer);
	}
private:
	void putTile()
	{
		int x = 0, y = 0;
		SDL_GetMouseState(&x, &y);
		x += wptr->camera.x;
		y += wptr->camera.y;

		for (int i = 0; i < m.totalTiles; i++)
		{
			if ((x > m.tileSet[i].box.x) && (x < m.tileSet[i].box.x + m.tileSet[i].box.w) && (y > m.tileSet[i].box.y) && (y < m.tileSet[i].box.y + m.tileSet[i].box.h))
				m.tileSet[i].setTile(m.tileSet[i].box.x, m.tileSet[i].box.y, currentType, m.tileInfo[currentType]);
		}
	}
	void saveMap()
	{
		std::stringstream file;
		file << mapFile << ".map";
		std::ofstream map(file.str());

		for (int i = 0; i < m.totalTiles; i++)
			map << m.tileSet[i].type << " ";

		map.close();
	}
private:
	Map m;
	bool quit;
	Uint32 currentFPS;
	int currentType;
	std::string mapFile;
};

Editor * eptr;