#pragma once
#include "Entity.h"

class Character
{
public:
	Character()
	{

	}
	~Character()
	{

	}
	void setCharacter(char * name, int xPos, int yPos)
	{
		entity.setEntity(name, xPos, yPos);
		entity.box.w = mptr->charTexture.getWidth();
		entity.box.h = mptr->charTexture.getHeight();
	}

public:
	Entity entity;
	bool moving = false;
};

Character * charptr;