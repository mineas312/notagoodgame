#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <fstream>

#include "Map.h"

int main(int argc, char* args[])
{
	wptr = new Window();

	//Initialize
	if (w.init() == false)
	{
		return 1;
	}

	//Load the files
	if (w.load_files() == false)
	{
		return 1;
	}

	w.loop();

	return 0;
}
