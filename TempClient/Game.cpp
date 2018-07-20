#include "stdafx.h"
#include "Shader.h"
#include "Game.h"
#include "Camera.h"
#include <map>

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
	glBindVertexArray(mptr->mapTilesTexture->vao);

	std::multimap<int, glm::mat4> map;

	std::mutex block;

	for (int i = 0; i < m.totalTiles; i++)
	{
		if (checkCollision(camptr->camRect, m.tileSet[i].box))
		{
			std::lock_guard<std::mutex> lg(block);
			map.insert(std::make_pair(m.tileSet[i].type, calcMVP(m.tileSet[i].box.x, m.tileSet[i].box.y)));
		}
	}
	GLuint lastTexture = -1;
	for (auto &kv : map)
	{
		if (lastTexture != mptr->mapTilesTexture[kv.first].texture)
		{
			glBindTexture(GL_TEXTURE_2D, mptr->mapTilesTexture[m.tileSet[kv.first].type].texture);
		}

		glUniformMatrix4fv(glGetUniformLocation(shadptr->ProgID, "model"), 1, GL_FALSE, glm::value_ptr(kv.second));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	}


}

bool checkCollision(const SDL_Rect & a, const SDL_Rect & b) noexcept
{
	return (a.y + a.h) <= b.y || (b.y + b.h) <= a.y || (a.x + a.w) <= b.x || (b.x + b.w) <= a.x ? false : true;
}

glm::mat4 calcMVP(int x, int y)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * winptr->rangePerWidthPixel, -y * winptr->rangePerHeightPixel, 0.0f));

	glm::mat4 mvp = camptr->proj * camptr->view * model;

	return mvp;
}