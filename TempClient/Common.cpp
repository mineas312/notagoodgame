#include "stdafx.h"
#include "Common.h"

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) noexcept
{
	return (a.y + a.h) <= b.y || (b.y + b.h) <= a.y || (a.x + a.w) <= b.x || (b.x + b.w) <= a.x ? false : true;
}

