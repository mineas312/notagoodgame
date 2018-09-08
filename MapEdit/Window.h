#pragma once
#include <SDL.h>
class Window
{
public:
	Window(const int width, const int height);

	bool init();

	void set_camera(int mapWidth, int mapHeight, Uint32 fps);

public:
	SDL_Event e;
	SDL_Rect camera;
	SDL_Window* window = nullptr;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
};

extern Window * wptr;