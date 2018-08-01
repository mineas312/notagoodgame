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
	netptr = new Network();
}

// -Initializates game components

void Game::init()
{
	winptr->init();
	netptr->init("localhost", 42069);
	shadptr->InitShader("res/shaders/tiles.vert", "res/shaders/tiles.frag");
	mptr->loadMedia();
	camptr->init();
	map.setMap(5120, 3840, "res/map1");

	charptr->setCharacter((char*)"Janusz", 0, 0);
	glClearColor(0.0, 0.0, 0.5, 1.0);

	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, gptr->ubo);
	glBufferData(GL_UNIFORM_BUFFER, 1025 * 4 * 4 * 4, NULL, GL_STREAM_DRAW);
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
	glClear(GL_COLOR_BUFFER_BIT);

	camptr->center(charptr->entity.box.x - winptr->SCREEN_WIDTH/2 - charptr->entity.box.w/2, charptr->entity.box.y - winptr->SCREEN_HEIGHT/2 + charptr->entity.box.h/2, map);

	glUseProgram(shadptr->ProgID);
	renderMap(map);

	mptr->charTexture.render(charptr->entity.box.x, charptr->entity.box.y);

	SDL_GL_SwapWindow(winptr->window);

}

void Game::update()
{
	if (charptr->moving)
	{
		charptr->move(map, currentFPS);
	}
}

Game* gptr;

void Game::renderMap(Map & m)
{
	// Tiles rendering
	glBindTexture(GL_TEXTURE_2D, mptr->mapTilesTexture->texture);

	__declspec(align(16)) std::unordered_map<GLuint, std::vector<glm::mat4>> uniqueMap;

	for (int i = 0; i < m.totalTiles; i++)
	{
		if (uniqueMap.find(mptr->mapTilesTexture[m.tileSet[i].type].vao) == uniqueMap.end())
		{
			if (checkCollision(camptr->camRect, m.tileSet[i].box))
			{
				uniqueMap.emplace(std::make_pair(mptr->mapTilesTexture[m.tileSet[i].type].vao, std::vector<glm::mat4>()));
				uniqueMap.at(mptr->mapTilesTexture[m.tileSet[i].type].vao).push_back(calcMVP(m.tileSet[i].box.x, m.tileSet[i].box.y));
			}
		}
		else
		{
			if (checkCollision(camptr->camRect, m.tileSet[i].box))
			{
				uniqueMap.at(mptr->mapTilesTexture[m.tileSet[i].type].vao).push_back(calcMVP(m.tileSet[i].box.x, m.tileSet[i].box.y));
			}
		}
	}

	glBindBuffer(GL_UNIFORM_BUFFER, gptr->ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, gptr->ubo);

	for (auto& kv : uniqueMap)
	{
		glBindVertexArray(kv.first);

		glBufferSubData(GL_UNIFORM_BUFFER, 0, kv.second.size() * 64, kv.second.data());

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL, kv.second.size());
	}

	// Objects rendering
	__declspec(align(16)) std::unordered_map<int, std::vector<glm::mat4>> uniqueMap2;

	for (int i = 0; i < m.objCount; i++) {
		if (uniqueMap2.find(m.objects[i].id) == uniqueMap2.end()) {
			if (checkCollision(camptr->camRect, m.objects[i].box))
			{
				uniqueMap2.emplace(std::make_pair(m.objects[i].id, std::vector<glm::mat4>()));
				uniqueMap2.at(m.objects[i].id).push_back(calcMVP(m.objects[i].box.x, m.objects[i].box.y));
			}
		}
		else {
			if (checkCollision(camptr->camRect, m.objects[i].box))
			{
				uniqueMap2.at(m.objects[i].id).push_back(calcMVP(m.objects[i].box.x, m.objects[i].box.y));
			}
		}
	}

	for (auto& kv : uniqueMap2) {
		glBindVertexArray(mptr->mapObjTextures[kv.first].vao);

		glBindTexture(GL_TEXTURE_2D, mptr->mapObjTextures[kv.first].texture);

		glBufferSubData(GL_UNIFORM_BUFFER, 0, kv.second.size() * 64, kv.second.data());

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL, kv.second.size());
	}
}