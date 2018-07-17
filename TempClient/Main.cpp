#include "stdafx.h"

#include "Game.h"

int main(int argc, char ** args)
{
#define _CRT_SECURE_NO_WARNINGS
	// Redirect stderr to file
	const FILE* pocoto = std::freopen("ErrorFile.log", "w", stderr);
#undef _CRT_SECURE_NO_WARNINGS
	gptr = new Game();

	gptr->init();
	gptr->loop();

	delete gptr;

    return 0;
}

