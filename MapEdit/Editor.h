#pragma once
#include "Map.h"
#include "GUI.h"

enum Mode
{
	TILES_EDITOR,
	OBJECT_EDITOR
};

class Editor
{
public:
	Editor(int screen_width, int screen_height)
	{
		wptr = new Window(screen_width, screen_height);
		quit = false;
		currentType = 0;
		currentTypeObject = 0;
		gptr = new GUI();
		mode = TILES_EDITOR;
		editorMoveThrough = false;
	}
	void init(std::string map, int mapWidth, int mapHeight)
	{
		if (!wptr->init())
		{
			exit(1);
		}
		m.setMap(map, mapWidth, mapHeight);
		mapFile = map;
		gptr->init(m);
	}
	void loop()
	{
		Uint64 startClock = SDL_GetPerformanceCounter();
		Uint64 lastClock = 0;

		while (!quit)
		{
			lastClock = startClock;
			startClock = SDL_GetPerformanceCounter();

			currentFPS = 1000.0 / ((startClock - lastClock) * 1000.0 / SDL_GetPerformanceFrequency());

			processEvents();

			render();
		}
		saveMap();
	}
	void render()
	{
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 128, 255);
		SDL_RenderClear(g_renderer);

		wptr->set_camera(m.width, m.height, currentFPS);

		// Render tiles
		for (int i = 0; i < m.totalTiles; i++)
		{
			if (check_collision(wptr->camera, m.tileSet[i].box))
				m.tileSet[i].render(&m.tilesPlace[m.tileSet[i].type]);
		}

		// Render objects
		for (Object & obj : m.objects)
		{
			if (check_collision(wptr->camera, obj.box))
				g_ObjectTextures[obj.id].render(obj.box.x - wptr->camera.x, obj.box.y - wptr->camera.y);
		}

		gptr->renderGUI(m, currentType, mode, currentTypeObject);

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
	void putObject()
	{
		int x = 0, y = 0;
		SDL_GetMouseState(&x, &y);
		x += wptr->camera.x;
		y += wptr->camera.y;

		Object newObj;
		newObj.set(g_ObjectTextures[currentTypeObject].width, g_ObjectTextures[currentTypeObject].height, x - g_ObjectTextures[currentTypeObject].width/2, y - g_ObjectTextures[currentTypeObject].height/2, editorMoveThrough, currentTypeObject);
		m.objects.push_back(newObj);
	}
	void saveMap()
	{
		std::stringstream file;
		file << mapFile << "/mapFile.map";
		std::ofstream map(file.str());

		for (int i = 0; i < m.totalTiles; i++)
			map << m.tileSet[i].type << " ";

		map.close();

		std::stringstream file2;
		file2 << mapFile << "/objects.txt";
		std::ofstream txt(file2.str());
		txt << m.objCount << " ";
		txt << m.objects.size() << " ";

		for (Object & obj : m.objects)
			txt << obj.box.w << " " << obj.box.h << " " << obj.box.x << " " << obj.box.y << " " << obj.canMoveThrough << " " << obj.id << "\n";
		txt.close();
	}
	void removeObject()
	{
		int x = 0, y = 0;
		SDL_GetMouseState(&x, &y);
		x += wptr->camera.x;
		y += wptr->camera.y;

		int i = 0;
		for (Object & obj : m.objects)
		{
			if ((x > obj.box.x) && (x < obj.box.x + obj.box.w) && (y > obj.box.y) && (y < obj.box.y + obj.box.h))
			{
				m.objects.erase(m.objects.begin() + i);
				break;
			}
			i++;
		}
	}
	void processEvents()
	{
		while (SDL_PollEvent(&wptr->e) != 0)
		{
			if (wptr->e.type == SDL_QUIT)
				quit = true;
			if (wptr->e.type == SDL_KEYDOWN)
			{
				switch (wptr->e.key.keysym.sym)
				{
				case SDLK_m:
				{
					if (mode == TILES_EDITOR)
					{
						mode = OBJECT_EDITOR;
						if (editorMoveThrough)
							SDL_SetWindowTitle(wptr->window, "Level Designer. Can move through object");
						else
							SDL_SetWindowTitle(wptr->window, "Level Designer. Can't move through object");
					}
					else
					{
						mode = TILES_EDITOR;
						std::stringstream title;
						title << "Level Designer. Current Tile ID: " << currentType;
						SDL_SetWindowTitle(wptr->window, title.str().c_str());
					}
				}
				case SDLK_b:
				{
					if (mode == OBJECT_EDITOR)
					{
						if (editorMoveThrough)
						{
							SDL_SetWindowTitle(wptr->window, "Level Designer. Can't move through object");
							editorMoveThrough = false;
						}
						else
						{
							SDL_SetWindowTitle(wptr->window, "Level Designer. Can move through object");
							editorMoveThrough = true;
						}
					}
				}
				}
			}
			if (wptr->e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (wptr->e.button.button == SDL_BUTTON_LEFT)
				{
					if (mode == TILES_EDITOR)
						putTile();
					else if (mode == OBJECT_EDITOR)
						putObject();
				}
				else if (wptr->e.button.button == SDL_BUTTON_RIGHT)
					removeObject();
			}
			if (wptr->e.type == SDL_MOUSEWHEEL)
			{
				if (wptr->e.wheel.y > 0) // scroll up
				{
					if (currentType + 1 < m.totalTileSetTiles && mode == TILES_EDITOR)
					{
						currentType++;
						std::stringstream title;
						title << "Level Designer. Current Tile ID: " << currentType;
						SDL_SetWindowTitle(wptr->window, title.str().c_str());
					}
					else if (currentTypeObject + 1 < m.objCount && mode == OBJECT_EDITOR)
						currentTypeObject++;
				}
				else if (wptr->e.wheel.y < 0) // scroll down
				{
					if (currentType >= 1 && mode == TILES_EDITOR)
					{
						currentType--;
						std::stringstream title;
						title << "Level Designer. Current Tile ID: " << currentType;
						SDL_SetWindowTitle(wptr->window, title.str().c_str());
					}
					else if (currentTypeObject >= 1 && mode == OBJECT_EDITOR)
						currentTypeObject--;
				}

			}
		}
	}
private:
	Map m;
	bool quit;
	Uint32 currentFPS;
	int currentType;
	int currentTypeObject;
	bool editorMoveThrough;
	std::string mapFile;
	Mode mode;
};

Editor * eptr;