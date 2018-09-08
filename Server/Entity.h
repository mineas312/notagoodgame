#pragma once

class Entity
{
public:
	Entity() noexcept : x{ 0 }, y{ 0 }, lastX{ 0 }, lastY{ 0 }, name { nullptr }
	{}

	void del() noexcept;

	void setEntity(char* _name, const int xPos, const int yPos) noexcept;

	void setPosition(int _x, int _y);

public:
	int x;
	int y;

	int lastX;
	int lastY;

	char* name;
};