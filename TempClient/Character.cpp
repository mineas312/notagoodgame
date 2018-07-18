#include "stdafx.h"
#include "Character.h"

void Character::setCharacter(char * name, const int xPos, const int yPos)
{
	entity.setEntity(name, xPos, yPos);
	entity.box.w = mptr->charTexture.texWidth;
	entity.box.h = mptr->charTexture.texHeight;
}

Character* charptr;
