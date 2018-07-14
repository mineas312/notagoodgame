#pragma once
#include <fstream>
#include "Window.h"
#include "Event.h"

class Game
{
public:
	Game()
	{
		winptr = new Window(800, 600);
		evptr = new Event();
		mptr = new Media();
		charptr = new Character();
	}

	// -Initializates game components
	void init()
	{
		winptr->init();
		mptr->loadMedia(winptr->renderer);

		// Map loading
		map.setMap(3200, 2400, "res/lazy", winptr->renderer);

		charptr->setCharacter(const_cast<char*>("Janusz"), 0, 0);
	}

	void loop()
	{
		Uint64 startClock = SDL_GetPerformanceCounter();
		Uint64 lastClock = 0;

		while (!quit)
		{
			// Fps counter
			lastClock = startClock;
			startClock = SDL_GetPerformanceCounter();

			currentFPS = 1000.0 / ((startClock - lastClock) * 1000.0 / (double)SDL_GetPerformanceFrequency());

			evptr->checkEvents(quit, charptr->moving);
			render();
			update();
		}
	}

	void close()
	{
		winptr->close();
		delete winptr;
		delete evptr;
	}

private:
	void render()
	{
		//SDL_SetRenderDrawColor(winptr->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		//SDL_RenderClear(winptr->renderer);

		winptr->centerCamera(charptr->entity.box.x, charptr->entity.box.y, map);

		for (int i = 0; i < map.totalTiles; i++)
			map.tileSet[i].render(winptr->renderer, winptr->camera, map.tilesPlace[map.tileSet[i].type]);

		mptr->charTexture.render(winptr->renderer, charptr->entity.box.x - winptr->camera.x, charptr->entity.box.y - winptr->camera.y);

		SDL_RenderPresent(winptr->renderer);
	}

	void update()
	{
		if (charptr->moving)
		{
			charptr->entity.move(map, currentFPS);
		}
	}

	double currentFPS;

	bool quit = false;

	//TODO Create component for maps
	Map map;
};

Game* gptr;
