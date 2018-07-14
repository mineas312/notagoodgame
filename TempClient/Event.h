#pragma once
#include <SDL.h>
#include "Keyboard.h"

class Event
{
public:
	__nothrow void checkEvents(bool& quit, bool& moving) noexcept;

private:
	SDL_Event e;
};

extern Event* evptr;
