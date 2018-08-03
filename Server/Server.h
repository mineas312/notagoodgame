#pragma once
#include <SDL.h>
#include "Network.h"

#define TICKRATE 32
#define MS_PER_TICK 1000/TICKRATE

class Server
{
public:
	Server() : quit{ false }
	{
		netptr = new Network;
	}

	void init();

private:
	void loop();

	void update();

	void second();

private:
	bool quit;
	int secCounter;
};