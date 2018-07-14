#pragma once
#include "Texture.h"
#include "TileMap.h"

class Entity
{
public:
	Entity()
	{
		xVel = 0;
		yVel = 0;
		name = (char*)"NULL";
		box.x = 0;
		box.y = 0;
		box.w = 16;
		box.h = 22;
	}
	~Entity()
	{

	}
	void setEntity(char* _name, int _xPos, int _yPos, TextureEnum _texture)
	{
		name = _name;
		box.x = _xPos;
		box.y = _yPos;
		texture = _texture;
	}
	void move(Map &m)
	{
		box.x += xVel;

		//If the dot went too far to the left or right or touched a wall
		if ((box.x < 0) || (box.x + box.w > m.MAP_WIDTH) || m.touchesWall(box))
		{
			//move back
			box.x -= xVel;
		}

		box.y += yVel;

		//If the dot went too far up or down or touched a wall
		if ((box.y < 0) || (box.y + box.h > m.MAP_HEIGHT) || m.touchesWall(box))
		{
			//move back
			box.y -= yVel;
		}
	}
private:
public:
	int xVel;
	int yVel;
	char* name;
	TextureEnum texture;
	SDL_Rect box;
private:
};