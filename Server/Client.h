#pragma once
#include <SDL_net.h>
#include "Entity.h"

struct Client
{
	IPaddress addr;
	Entity entity;
	bool used = false;
	int timeout = 0;
};

