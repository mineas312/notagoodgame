#include "stdafx.h"

#include "Event.h"
#include "Keyboard.h"

void Event::checkEvents(bool & quit, bool & moving)
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
			case SDLK_w:
			{
				keys[W] = true;
				moving = true;
				processKeyEvents(moving);
				break;
			}
			case SDLK_s:
			{
				keys[S] = true;
				moving = true;
				processKeyEvents(moving);
				break;
			}
			case SDLK_a:
			{
				keys[A] = true;
				moving = true;
				processKeyEvents(moving);
				break;
			}
			case SDLK_d:
			{
				keys[D] = true;
				moving = true;
				processKeyEvents(moving);
				break;
			}
			default: 
				break;
			}
		}

		//If a key was released
		else if (e.type == SDL_KEYUP)
		{
			//Adjust the velocity
			switch (e.key.keysym.sym)
			{
			case SDLK_w:
			{
				keys[W] = false;
				processKeyEvents(moving);
				break;
			}
			case SDLK_s:
			{
				keys[S] = false;
				processKeyEvents(moving);
				break;
			}
			case SDLK_a:
			{
				keys[A] = false;
				processKeyEvents(moving);
				break;
			}
			case SDLK_d:
			{
				keys[D] = false;
				processKeyEvents(moving);
				break;
			}
			default: 
				break;
			}
		}
	}
}

Event* evptr;
