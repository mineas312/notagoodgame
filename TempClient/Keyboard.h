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

extern bool keys[TOTAL_KEYS];

void processKeyEvents(bool & moving) noexcept;
