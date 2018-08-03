#pragma once
#include <SDL.h>

class Entity
{
public:
	Entity() noexcept : x{ 0 }, y{ 0 }, name{ NULL }
	{}

	void del() noexcept;

	void setEntity(char* _name, const int xPos, const int yPos) noexcept;

	void setPosition(int _x, int _y);

public:
	int x;
	int y;

	char* name;
};