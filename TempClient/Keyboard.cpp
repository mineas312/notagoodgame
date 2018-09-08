#include "stdafx.h"

#include "Keyboard.h"
#include "Character.h"

void processKeyEvents(bool & moving) noexcept
{
	if (keys[A])
		charptr->xSpeed = -300.0;

	if (keys[D])
		charptr->xSpeed = 300.0;

	if (keys[A] && keys[D])
		charptr->xSpeed = 0.0;

	if (!keys[A] && !keys[D])
		charptr->xSpeed = 0.0;

	if (keys[W])
		charptr->ySpeed = -300.0;

	if (keys[S])
		charptr->ySpeed = 300.0;

	if (keys[W] && keys[S])
		charptr->ySpeed = 0.0;

	if (!keys[W] && !keys[S])
		charptr->ySpeed = 0.0;

	if (!keys[W] && !keys[S] && !keys[A] && !keys[D])
		moving = false;
}

bool keys[TOTAL_KEYS];
