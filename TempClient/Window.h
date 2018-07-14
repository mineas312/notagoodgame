#pragma once
#include "Map.h"

class Window
{
public:
	// -Sets width and height of window
	Window(const int width, const int height) : SCREEN_WIDTH{ width }, SCREEN_HEIGHT{ height }, window{ NULL }, renderer{ NULL }, camera{NULL}
	{}

	// -Initializes window, renderer
	void init();

	void centerCamera(const int x, const int y, Map& map);

	void close();

public:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect camera;
};

extern Window* winptr;
