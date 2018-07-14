#pragma once
#include "Character.h"

enum KeyBoard
{
	W,
	A,
	S,
	D,
	TOTAL_KEYS
};

bool keys[TOTAL_KEYS];

void processKeyEvents(bool & moving)
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
