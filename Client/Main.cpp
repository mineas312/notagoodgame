#define _CRT_SECURE_NO_WARNINGS
#include "Client.h"

int main(int argc, char** argv)
{
	// Redict strerr to file
	freopen("ErrorFile.log", "w", stderr);

	Client * c = new Client();
	c->start();

	return 0;
}