#pragma once
#include <SDL.h>

class Event
{
public:
	Event() : e()
	{}
	void checkEvents(bool& quit, bool& moving);

private:
	SDL_Event e;
};

extern Event* evptr;
