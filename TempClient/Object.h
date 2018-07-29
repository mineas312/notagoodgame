#pragma once
#include <SDL.h>

class Object
{
public:
	Object() : canMoveThrough{ false }
	{}

	void set(int width, int height, int x, int y, bool _canMoveThrough, int _id);

public:
	SDL_Rect box;
	bool canMoveThrough;
	int id;
};