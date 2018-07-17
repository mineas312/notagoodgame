#include "stdafx.h"

#include "Keyboard.h"

void processKeyEvents(bool & moving) noexcept
{
	if (keys[A])
		charptr->entity.xSpeed = -300.0;

	if (keys[D])
		charptr->entity.xSpeed = 300.0;

	if (keys[A] && keys[D])
		charptr->entity.xSpeed = 0.0;

	if (!keys[A] && !keys[D])
		charptr->entity.xSpeed = 0.0;

	if (keys[W])
		charptr->entity.ySpeed = -300.0;

	if (keys[S])
		charptr->entity.ySpeed = 300.0;

	if (keys[W] && keys[S])
		charptr->entity.ySpeed = 0.0;

	if (!keys[W] && !keys[S])
		charptr->entity.ySpeed = 0.0;

	if (!keys[W] && !keys[S] && !keys[A] && !keys[D])
		moving = false;
}

bool keys[TOTAL_KEYS];
