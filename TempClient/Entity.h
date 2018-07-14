#pragma once
#include <SDL.h>
#include "Map.h"

class Entity
{
public:
	Entity() : name{ NULL }, xSpeed{ 0.0 }, ySpeed{ 0.0 }, rxSpeed{ 0.0 }, rySpeed{ 0.0 }
	{}

	void del()
	{
		delete[] name;
		name = NULL;
	}

	void setEntity(char* _name, int xPos, int yPos)
	{
		name = _name;
		box.x = xPos;
		box.y = yPos;

		// TODO DELETE
		box.w = 16;
		box.h = 22;
	}

	void move(Map &map, double fps)
	{
		if (fps == 0.0)
			return;

		// Dnia 14.07.2018 g³ówny programista nie wiedzia³ jak uniezale¿niæ prêdkoœæ ruchu od FPSów
		double tmpX = (rxSpeed + xSpeed / fps); // By³o (rxSpeed + xSpeed) / fps XDDDD
		rxSpeed = std::modf(tmpX, &tmpX);

		double tmpY = (rySpeed + ySpeed / fps);
		rySpeed = std::modf(tmpY, &tmpY);

		box.x += static_cast<int>(tmpX);
		if ((box.x < 0) || (box.x + box.w > map.width) || map.collides(box))
			box.x -= static_cast<int>(tmpX);

		box.y += static_cast<int>(tmpY);
		if ((box.y < 0) || (box.y + box.w > map.height) || map.collides(box))
			box.y -= static_cast<int>(tmpY);
	}

public:
	SDL_Rect box;
	double xSpeed;
	double ySpeed;
	double rxSpeed;
	double rySpeed;

	char* name;
};
