#pragma once
#include "Graphics.h"

class Client
{
public:
	Client()
	{
		startclock = 0;
		deltaclock = 0;
		currentFPS = 0;
		gptr = new Graphics();
	}
	~Client()
	{
		delete gptr;
		gptr = NULL;
	}
	// -Triggers needed components and starts client
	void start()
	{
		gptr->init();
		loop();
	}
private:
	void loop()
	{
		startclock = SDL_GetTicks();
		while (!quit)
		{
			deltaclock = SDL_GetTicks() - startclock;
			if (deltaclock != 0)
				currentFPS = 1000 / deltaclock;

			while (SDL_PollEvent(&gptr->e) != 0)
			{
				if (gptr->e.type == SDL_QUIT)
					quit = true;
				handleEvents(gptr->e);
			}
			while (SDL_GetTicks() - startclock >= 1000 / 75)
			{
				update();
				startclock += 1000 / 75;
			}
			render();
		}
	}
	void update()
	{
		enptr->moveEntities(enptr->maps[enptr->currentMap]);
	}
	void render()
	{
		gptr->render();
	}
	void handleEvents(SDL_Event& e)
	{
		//If a key was pressed
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
		{
			//Adjust the velocity
			switch (e.key.keysym.sym)
			{
			case SDLK_UP: enptr->character.yVel -= 3; break;
			case SDLK_DOWN: enptr->character.yVel += 3; break;
			case SDLK_LEFT: enptr->character.xVel -= 3; break;
			case SDLK_RIGHT: enptr->character.xVel += 3; break;
			}
		}
		//If a key was released
		else if (e.type == SDL_KEYUP && e.key.repeat == 0)
		{
			//Adjust the velocity
			switch (e.key.keysym.sym)
			{
			case SDLK_UP: enptr->character.yVel += 3; break;
			case SDLK_DOWN: enptr->character.yVel -= 3; break;
			case SDLK_LEFT: enptr->character.xVel += 3; break;
			case SDLK_RIGHT: enptr->character.xVel -= 3; break;
			}
		}
	}
private:
	Uint32 startclock;
	Uint32 deltaclock;
	Uint32 currentFPS;
	Graphics * gptr;
	bool quit = false;
};

