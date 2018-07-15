#pragma once
#include "Map.h"

class Entity
{
public:
	Entity() noexcept : box{ 0,0,16,22 }, name { NULL }, xSpeed{ 0.0 }, ySpeed{ 0.0 }, rxSpeed{ 0.0 }, rySpeed{ 0.0 }
	{}

	void del();

	void setEntity(char* _name, const int xPos, const int yPos);

	void move(Map &map, const double fps);

public:
	SDL_Rect box;
	double xSpeed;
	double ySpeed;
	double rxSpeed;
	double rySpeed;

	char* name;
};
