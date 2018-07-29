#pragma once
#include <SDL.h>

class Object
{
public:
	Object() : canMoveThrough{ false }, id{ -1 }
	{}

	void set(int width, int height, int x, int y, bool _canMoveThrough, int _id)
	{
		box.w = width;
		box.h = height;
		box.x = x;
		box.y = y;
		canMoveThrough = _canMoveThrough;
		id = _id;
	}

public:
	SDL_Rect box;
	bool canMoveThrough;
	int id;
};