#pragma once
#include "Map.h"

class Entity
{
public:
	Entity() noexcept : box{ 0,0,16,22 }, name { NULL }, xSpeed{ 0.0 }, ySpeed{ 0.0 }, rxSpeed{ 0.0 }, rySpeed{ 0.0 }
	{}

	void del() noexcept;

	void setEntity(char* _name, const int xPos, const int yPos) noexcept;

	void move(Map &map, const double fps);

public:
	SDL_Rect box;
	double xSpeed;
	double ySpeed;
	double rxSpeed;
	double rySpeed;

	char* name;
};
