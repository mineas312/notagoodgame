#pragma once
#include "Entity.h"

class Character
{
public:
	void setCharacter(char* name, const int xPos, const int yPos) noexcept;

public:
	Entity entity;
	bool moving = false;
};

extern Character* charptr;
