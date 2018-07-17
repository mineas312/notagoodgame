#include "stdafx.h"

#include "Game.h"

Game::Game() noexcept
{
	winptr = new Window(1600, 900);
	evptr = new Event();
	mptr = new Media();
	charptr = new Character();
}

// -Initializates game components

void Game::init()
{
	winptr->init();
	//mptr->loadMedia(winptr->renderer);

	// Map loading
	//map.setMap(5120, 3840, "res/lazy", winptr->renderer);

	charptr->setCharacter((char*)"Janusz", 0, 0);
	//glClearColor(1.0, 1.0, 1.0, 1.0);
}

void Game::loop()
{
	Uint64 startClock = SDL_GetPerformanceCounter();
	Uint64 lastClock = 0;

	while (!quit)
	{
		// Fps counter
		lastClock = startClock;
		startClock = SDL_GetPerformanceCounter();

		currentFPS = 1000.0 / ((startClock - lastClock) * 1000.0 / static_cast<double>(SDL_GetPerformanceFrequency()));

		evptr->checkEvents(quit, charptr->moving);
		render();
		update();
	}
}

void Game::close() noexcept
{
	winptr->close();
	delete winptr;
	delete evptr;
}

void Game::render() noexcept
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(winptr->vao);

	// Bind program
	glUseProgram(winptr->programID);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	/*winptr->centerCamera(charptr->entity.box.x, charptr->entity.box.y, map);

	for (int i = 0; i < map.totalTiles; i++)
		map.tileSet[i].render(winptr->renderer, winptr->camera, map.tilesPlace[map.tileSet[i].type]);

	mptr->charTexture.render(winptr->renderer, charptr->entity.box.x - winptr->camera.x, charptr->entity.box.y - winptr->camera.y);*/

	SDL_GL_SwapWindow(winptr->window);
}

void Game::update()
{
	if (charptr->moving)
	{
		charptr->entity.move(map, currentFPS);
	}
}

Game* gptr;
