#include "Server.h"

void Server::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
		exit(-1);
	}

	netptr->init(42069);

	loop();
}

void Server::loop()
{
	Uint32 start = SDL_GetTicks();
	while (!quit)
	{
		netptr->recv();
		while (SDL_GetTicks() - start >= MS_PER_TICK)
		{
			update();
			start += MS_PER_TICK;
		}
	}
}

void Server::update()
{
	second();
}

void Server::second()
{
	if (secCounter < TICKRATE)
	{
		secCounter++;
		return;
	}
	else
	{
		for (int i = 0; i < SERVER_SLOTS; i++)
		{
			if (!netptr->clients[i].used)
				continue;
			else
			{
				netptr->clients[i].timeout++;
				if (netptr->clients[i].timeout > 5)
					netptr->disconnectClient(i);
			}
		}
		secCounter = 0;
	}
}
