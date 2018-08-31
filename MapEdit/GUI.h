#pragma once
#include "Map.h"

class GUI
{
public:
	void renderGUI(Map& m, int currentID, int mode, int currentObjID);

	void init(Map& m);

	void selectRect(SDL_Rect* rect);

private:
	SDL_Rect backgroudGUI = { wptr->SCREEN_WIDTH - 256, 0, 256, wptr->SCREEN_HEIGHT };
	SDL_Rect* tilesPosition = nullptr;
	SDL_Rect* objectsPosition = nullptr;
};

extern GUI* gptr;