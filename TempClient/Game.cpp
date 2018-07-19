#include "stdafx.h"
#include "Shader.h"
#include "Game.h"
#include "Camera.h"

Game::Game() noexcept
{
	winptr = new Window(1600, 900);
	evptr = new Event();
	mptr = new Media();
	charptr = new Character();
	shadptr = new Shader();
	camptr = new Camera();
}

// -Initializates game components

void Game::init()
{
	winptr->init();
	shadptr->InitShader("res/shaders/vs.txt", "res/shaders/fs.txt");
	mptr->loadMedia();
	camptr->init();

	//Map loading
	map.setMap(5120, 3840, "res/lazy");

	charptr->setCharacter((char*)"Janusz", 0, 0);
	//glClearColor(1.0, 1.0, 1.0, 1.0);
}

void Game::loop()
{
	Uint64 startClock = SDL_GetPerformanceCounter();
	Uint64 lastClock = 0;

	glUseProgram(shadptr->ProgID);

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

	camptr->center(charptr->entity.box.x, -charptr->entity.box.y);

	for (int i = 0; i < map.totalTiles; i++)
		map.tileSet[i].render();

	mptr->charTexture.render(charptr->entity.box.x, charptr->entity.box.y);

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
