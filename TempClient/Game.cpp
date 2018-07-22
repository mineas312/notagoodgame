#include "stdafx.h"
#include "Shader.h"
#include "Game.h"
#include "Camera.h"
#include "Common.h"

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
	glClearColor(0.0, 0.0, 0.5, 1.0);
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

	camptr->center(charptr->entity.box.x - winptr->SCREEN_WIDTH/2, charptr->entity.box.y - winptr->SCREEN_HEIGHT/2, map);

	renderTiles(map);

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

void renderTiles(Map &m)
{
	glBindTexture(GL_TEXTURE_2D, mptr->mapTilesTexture->texture);

	std::multimap<GLuint, glm::mat4> map;

	for (int i = 0; i < m.totalTiles; i++)
	{
		if (checkCollision(camptr->camRect, m.tileSet[i].box))
		{
			map.insert(std::pair<GLuint, glm::mat4>(mptr->mapTilesTexture[m.tileSet[i].type].vao, calcMVP(m.tileSet[i].box.x, m.tileSet[i].box.y)));
		}
	}

	GLuint lastVAO = -1;
	for (auto &kv : map)
	{
		if (lastVAO != mptr->mapTilesTexture[kv.first].vao)
		{
			glBindVertexArray(kv.first);
			lastVAO = mptr->mapTilesTexture[m.tileSet[kv.first].type].vao;
		}

		glUniformMatrix4fv(glGetUniformLocation(shadptr->ProgID, "model"), 1, GL_FALSE, glm::value_ptr(kv.second));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	}

}
