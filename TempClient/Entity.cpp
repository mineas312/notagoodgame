#include "stdafx.h"

#include "Entity.h"

void Entity::del() noexcept
{
	delete[] name;
	name = NULL;
}

void Entity::setEntity(char * _name, const int xPos, const int yPos) noexcept
{
	name = _name;
	box.x = xPos;
	box.y = yPos;
}
