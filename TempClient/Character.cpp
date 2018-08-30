#include "stdafx.h"
#include "Character.h"
#include "Common.h"

void Character::setCharacter(char * name, const int xPos, const int yPos)
{
	entity.setEntity(name, xPos, yPos);
	entity.box.w = mptr->charTexture.texWidth;
	entity.box.h = mptr->charTexture.texHeight;
}

void Character::move(Map & map, const double fps)
{
	if (isEqf(fps, 0.0, 5))	// floating point equality comparisons require use of the hardware epsilon
		return;

	// Dnia 14.07.2018 g³ówny programista nie wiedzia³ jak uniezale¿niæ prêdkoœæ ruchu od FPSów
	double tmpX = (rxSpeed + xSpeed / fps);
	rxSpeed = std::modf(tmpX, &tmpX);

	double tmpY = (rySpeed + ySpeed / fps);
	rySpeed = std::modf(tmpY, &tmpY);

	entity.box.x += static_cast<int>(tmpX);
	if ((entity.box.x < 0) || (entity.box.x + entity.box.w > map.width) || map.collides(entity.box))
		entity.box.x -= static_cast<int>(tmpX);

	entity.box.y += static_cast<int>(tmpY);
	if ((entity.box.y < 0) || (entity.box.y + entity.box.w > map.height) || map.collides(entity.box))
		entity.box.y -= static_cast<int>(tmpY);
}

Character* charptr;
