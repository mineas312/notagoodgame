#include "Editor.h"

int main(int argc, char* args[])
{
	eptr = new Editor(1280, 720);

	eptr->init("res/map1", 5120, 3840);
	eptr->loop();

	return 0;
}
