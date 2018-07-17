#pragma once
#include <SDL.h>
#include "Keyboard.h"

class Event
{
public:
	void checkEvents(bool& quit, bool& moving);

private:
	SDL_Event e;
};

extern Event* evptr;
