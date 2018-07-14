#pragma once
#include "Entity.h"
#include <vector>

class Engine
{
public:
	Engine()
	{
		setCharacter();
		loadMaps();
	}
	~Engine()
	{

	}
	void moveEntities(Map &m)
	{
		character.move(m);
		for (Entity &en : entities)
			en.move(m);
	}
private:
	void setCharacter()
	{
		char name[] = "Player";
		character.setEntity(name, 0, 0, CHARACTER);
	}
	void loadMaps()
	{
		maps[DEBUG].setMap(1280, 960, "res/lazy.map");
	}
public:
	Entity character;
	std::vector<Entity> entities;
	Map maps[TOTAL_MAPS];
	MapEnum currentMap = DEBUG;
};

Engine * enptr;