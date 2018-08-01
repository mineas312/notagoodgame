#pragma once
#include "Window.h"
#include "Event.h"
#include "Network.h"

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

	void renderMap(Map &m);

public:
	GLuint ubo;

private:

	double currentFPS = 0.0;

	bool quit = false;

	//TODO Create component for maps
	Map map;


};

extern Game* gptr;