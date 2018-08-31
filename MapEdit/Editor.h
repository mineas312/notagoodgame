#pragma once
#include "Map.h"
#include "GUI.h"

enum Mode
{
	TILES_EDITOR,
	OBJECT_EDITOR
};

class Editor
{
public:
	Editor(int screen_width, int screen_height);

	void init(std::string map, int mapWidth, int mapHeight);

	void loop();

	void render();

private:
	void putTile();

	void putObject();

	void saveMap();

	void removeObject();

	void processEvents();

	void replaceTiles();

private:
	Map m;
	bool quit = false;
	Uint32 currentFPS;
	int currentType = 0;
	int currentTypeObject = 0;
	bool editorMoveThrough = false;
	std::string mapFile;
	Mode mode = TILES_EDITOR;
	bool rightClick = false;
	SDL_Rect select;
};

extern Editor* eptr;