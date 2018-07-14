#pragma once
#include <SDL.h>
#include "Keyboard.h"

class Event
{
public:
	void checkEvents(bool& quit, bool& moving)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;

			//If a key was pressed
			if (e.type == SDL_KEYDOWN)
			{
				//Adjust the velocity
				switch (e.key.keysym.sym)
				{
					case SDLK_UP:
					{
						keys[W] = true;
						moving = true;
						processKeyEvents(moving);
						break;
					}
					case SDLK_DOWN:
					{
						keys[S] = true;
						moving = true;
						processKeyEvents(moving);
						break;
					}
					case SDLK_LEFT:
					{
						keys[A] = true;
						moving = true;
						processKeyEvents(moving);
						break;
					}
					case SDLK_RIGHT:
					{
						keys[D] = true;
						moving = true;
						processKeyEvents(moving);
						break;
					}
				}
			}

			//If a key was released
			else if (e.type == SDL_KEYUP)
			{
				//Adjust the velocity
				switch (e.key.keysym.sym)
				{
					case SDLK_UP: 
					{
						keys[W] = false;
						processKeyEvents(moving);
						break;
					}
					case SDLK_DOWN:
					{
						keys[S] = false;
						processKeyEvents(moving);
						break;
					}
					case SDLK_LEFT:
					{
						keys[A] = false;
						processKeyEvents(moving);
						break;
					}
					case SDLK_RIGHT:
					{
						keys[D] = false;
						processKeyEvents(moving);
						break;
					}
				}
			}
		}
	}

private:
	SDL_Event e;
};

Event* evptr;
