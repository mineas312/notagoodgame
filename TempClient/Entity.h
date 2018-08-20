#pragma once
#include "Map.h"

class Entity
{
public:
	Entity() noexcept : box{ 0, 0, 0, 0 }, name { NULL }, id{ -1 }
	{}

	void del() noexcept;

	void setEntity(char* _name, const int xPos, const int yPos) noexcept;

public:
	SDL_Rect box;

	int id;

	char* name;
};
