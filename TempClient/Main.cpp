#include "stdafx.h"

#define _CRT_SECURE_NO_WARNINGS
#include "Game.h"

int main(int argc, char ** args)
{
	// Redirect strerr to file
	FILE* pocoto = freopen("ErrorFile.log", "w", stderr);

	gptr = new Game();

	gptr->init();
	gptr->loop();

	delete gptr;

    return 0;
}

