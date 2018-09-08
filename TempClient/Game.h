#pragma once
#include "Entity.h"

class Game
{
public:
	Game() noexcept;

	// -Initializates game components
	void init();

	void loop();

	void close() const noexcept;

private:
	void render() noexcept;

	void update();

	void renderMap(Map &m) const;

	void second();

public:
	GLuint ubo{};

private:

	double currentFPS = 0.0;

	bool quit = false;

	//TODO Create component for maps
	Map map;

	int secCounter = 0;

	std::vector<Entity> entities;
};

extern Game* gptr;