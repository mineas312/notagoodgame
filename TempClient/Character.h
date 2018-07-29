#pragma once
#include "Entity.h"

class Character
{
public:
	Character() : xSpeed{ 0.0 }, ySpeed{ 0.0 }, rxSpeed{ 0.0 }, rySpeed{ 0.0 }
	{}

	void setCharacter(char* name, const int xPos, const int yPos);

	void move(Map & map, const double fps);

public:
	Entity entity;
	bool moving = false;
	double xSpeed;
	double ySpeed;
	double rxSpeed;
	double rySpeed;
};

extern Character* charptr;
