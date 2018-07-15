#include "stdafx.h"

#include "Entity.h"

void Entity::del()
{
	delete[] name;
	name = NULL;
}

void Entity::setEntity(char * _name, const int xPos, const int yPos)
{
	name = _name;
	box.x = xPos;
	box.y = yPos;
}

void Entity::move(Map & map, const double fps)
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
