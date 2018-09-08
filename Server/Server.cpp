#include "Server.h"
#include <cstdio>

void Server::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error initializing SDL: %s\n", SDL_GetError());
		exit(-1);
	}

	netptr->init(42069);

	printf("[INFO] Server initialization complete\n");

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
	for (int i = 0; i < SERVER_SLOTS; i++)
	{
		if (netptr->clients[i].used)
		{
			for (int j = 0; j < SERVER_SLOTS; j++)
			{
				if (netptr->clients[j].used)
				{
					if (j != i)
					{
						if (netptr->clients[j].entity.lastX == netptr->clients[j].entity.x && netptr->clients[j].entity.lastY == netptr->clients[j].entity.y)
							continue;
						Uint8 data[12];
						netptr->intToUint8(netptr->clients[j].entity.x, &data[0]);
						netptr->intToUint8(netptr->clients[j].entity.y, &data[4]);
						netptr->intToUint8(j, &data[8]);
						netptr->send(netptr->clients[i], 12, (char*)data, ENTITY_POSITION);
						netptr->clients[j].entity.lastX = netptr->clients[j].entity.x;
						netptr->clients[j].entity.lastY = netptr->clients[j].entity.y;
					}
				}
			}
		}
	}
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
