#include "Entity.h"

void Entity::del() noexcept
{
	delete[] name;
	name = nullptr;
}

void Entity::setEntity(char * _name, const int xPos, const int yPos) noexcept
{
	name = _name;
	x = xPos;
	y = yPos;
}

void Entity::setPosition(int _x, int _y)
{
	x = _x;
	y = _y;
}
