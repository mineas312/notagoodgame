#pragma once
#include "Map.h"

class Entity
{
public:
	Entity() noexcept : box{ 0, 0, 0, 0 }, id{ -1 }, name{ nullptr }, interTicks{ 0 }, interRemainX{ 0.0f }, interRemainY{ 0.0f }, sumX{ 0.0f }, interX{ 0 }, sumY{ 0.0f }, interY{ 0 }
	{}

	void del() noexcept;

	void setEntity(char* _name, const int xPos, const int yPos) noexcept;

	void moveInterpolated(int _x, int _y);

	void tickInterpolated();

public:
	SDL_Rect box;

	int id;

	char* name;

	int interTicks;

	int interX, interY;

	float interRemainX, interRemainY;

	float sumX, sumY;
};
