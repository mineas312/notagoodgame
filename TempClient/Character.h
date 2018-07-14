#pragma once
#include "Entity.h"

class Character
{
public:
	void setCharacter(char* name, const int xPos, const int yPos)
	{
		entity.setEntity(name, xPos, yPos);
		entity.box.w = mptr->charTexture.getWidth();
		entity.box.h = mptr->charTexture.getHeight();
	}

public:
	Entity entity;
	bool moving = false;
};

Character* charptr;
