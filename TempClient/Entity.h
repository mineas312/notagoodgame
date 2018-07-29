#pragma once
#include "Map.h"

class Entity
{
public:
	Entity() noexcept : box{ 0, 0, 0, 0 }, name { NULL }
	{}

	void del() noexcept;

	void setEntity(char* _name, const int xPos, const int yPos) noexcept;

public:
	SDL_Rect box;

	char* name;
};
