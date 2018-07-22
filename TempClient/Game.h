#pragma once
#include "Window.h"
#include "Event.h"

void renderTiles(Map &m);

class Game
{
public:
	Game() noexcept;

	// -Initializates game components
	void init();

	void loop();

	void close() noexcept;

private:
	void render() noexcept;

	void update();

private:

	double currentFPS = 0.0;

	bool quit = false;

	//TODO Create component for maps
	Map map;
};

extern Game* gptr;