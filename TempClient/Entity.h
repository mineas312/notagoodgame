#pragma once
#include <SDL.h>
#include "Map.h"

class Entity
{
public:
	Entity()
	{
		name = NULL;
		xSpeed = 0.0f;
		ySpeed = 0.0f;
		rxSpeed = 0.0f;
		rySpeed = 0.0f;
	}
	~Entity()
	{

	}
	void del()
	{
		delete[] name;
		name = NULL;
	}
	void setEntity(char * name, int xPos, int yPos)
	{
		this->name = name;
		box.x = xPos;
		box.y = yPos;

		// TODO DELETE
		box.w = 16;
		box.h = 22;
	}
	void move(Map &map, Uint32 fps)
	{
		if (fps == 0)
			return;

		float tmpX = (rxSpeed + xSpeed) / fps;
		rxSpeed = std::modf(tmpX, &tmpX);

		float tmpY = (rySpeed + ySpeed) / fps;
		rySpeed = std::modf(tmpY, &tmpY);

		box.x += tmpX;

		if ((box.x < 0) || (box.x + box.w > map.width) || map.collides(box))
			box.x -= tmpX;

		box.y += tmpY;

		if ((box.y < 0) || (box.y + box.w > map.height) || map.collides(box))
			box.y -= tmpY;
	}
public:
	SDL_Rect box;
	float xSpeed;
	float ySpeed;
	char * name;
	float rxSpeed;
	float rySpeed;
};