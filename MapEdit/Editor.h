#pragma once
#include "Map.h"

enum Mode
{
	TILES_EDITOR,
	OBJECT_EDITOR
};

class Editor
{
public:
	Editor(int screen_width, int screen_height);

	void init(std::string map, int map_width, int map_height);

	void loop();

	void render();

private:
	void put_tile() const;

	void put_object();

	void save_map();

	void remove_object();

	void process_events();

	void replace_tiles();

private:
	Map m_;
	bool quit_ = false;
	Uint32 current_fps_;
	int current_type_ = 0;
	int current_type_object_ = 0;
	bool editor_move_through_ = false;
	std::string map_file_;
	Mode mode_ = TILES_EDITOR;
	bool right_click_ = false;
	SDL_Rect select_{};
};

extern Editor* eptr;