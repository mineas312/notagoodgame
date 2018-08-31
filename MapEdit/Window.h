#pragma once
#include <SDL.h>
class Window
{
public:
	Window(const int width, const int height);

	bool init();

	void set_camera(int mapWidth, int mapHeight, Uint32 fps);

private:
	SDL_Surface* screen = nullptr;
	bool quit = false;
	int xSpeed = 3;
	int ySpeed = 3;
	float rxSpeed = 0.0f;
	float rySpeed = 0.0f;

public:
	SDL_Event e;
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH - 256, SCREEN_HEIGHT };
	SDL_Window* window = nullptr;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
};

extern Window * wptr;