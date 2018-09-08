#include "Editor.h"
#include "GUI.h"
#include <fstream>
#include <sstream>

Editor::Editor(int screen_width, int screen_height)
{
	wptr = new Window(screen_width, screen_height);
	gptr = new GUI();
	current_fps_ = 0;

}

void Editor::init(const std::string map, int map_width, int mapHeight)
{
	if (!wptr->init())
	{
		exit(1);
	}

	m_.setMap(map, map_width, mapHeight);
	map_file_ = map;
	gptr->init(m_);
}

void Editor::loop()
{
	Uint64 start_clock = SDL_GetPerformanceCounter();
	Uint64 last_clock;

	while (!quit_)
	{
		last_clock = start_clock;
		start_clock = SDL_GetPerformanceCounter();

		current_fps_ = 1000.0 / ((start_clock - last_clock) * 1000.0 / SDL_GetPerformanceFrequency());

		process_events();

		render();
	}

	save_map();
}

void Editor::render()
{
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 128, 255);
	SDL_RenderClear(g_renderer);

	wptr->set_camera(m_.width, m_.height, current_fps_);

	// Render tiles
	for (int i = 0; i < m_.totalTiles; i++)
	{
		if (check_collision(wptr->camera, m_.tileSet[i].box))
			m_.tileSet[i].render(&m_.tilesPlace[m_.tileSet[i].type]);
	}

	// Render objects
	for (Object& obj : m_.objects)
	{
		if (check_collision(wptr->camera, obj.box))
			g_ObjectTextures[obj.id].render(obj.box.x - wptr->camera.x, obj.box.y - wptr->camera.y);
	}

	gptr->renderGUI(m_, current_type_, mode_, current_type_object_);

	if (right_click_)
		gptr->selectRect(&select_);

	SDL_RenderPresent(g_renderer);
}

void Editor::put_tile() const
{
	int x = 0, y = 0;
	SDL_GetMouseState(&x, &y);
	x += wptr->camera.x;
	y += wptr->camera.y;

	for (int i = 0; i < m_.totalTiles; i++)
	{
		if ((x > m_.tileSet[i].box.x) && (x < m_.tileSet[i].box.x + m_.tileSet[i].box.w) && (y > m_.tileSet[i].box.y) &&
			(y < m_.tileSet[i].box.y + m_.tileSet[i].box.h))
			m_.tileSet[i].set_tile(m_.tileSet[i].box.x, m_.tileSet[i].box.y, current_type_, m_.tileInfo[current_type_]);
	}
}

void Editor::put_object()
{
	int x = 0, y = 0;
	SDL_GetMouseState(&x, &y);
	x += wptr->camera.x;
	y += wptr->camera.y;

	Object newObj;
	newObj.set(g_ObjectTextures[current_type_object_].width, g_ObjectTextures[current_type_object_].height,
	           x - g_ObjectTextures[current_type_object_].width / 2,
	           y - g_ObjectTextures[current_type_object_].height / 2, editor_move_through_, current_type_object_);
	m_.objects.push_back(newObj);
}

void Editor::save_map()
{
	std::stringstream file;
	file << map_file_ << "/mapFile.map";
	std::ofstream map(file.str());

	for (int i = 0; i < m_.totalTiles; i++)
		map << m_.tileSet[i].type << " ";

	map.close();

	std::stringstream file2;
	file2 << map_file_ << "/objects.txt";
	std::ofstream txt(file2.str());
	txt << m_.objCount << " ";
	txt << m_.objects.size() << " ";

	for (const Object& obj : m_.objects)
		txt << obj.box.w << " " << obj.box.h << " " << obj.box.x << " " << obj.box.y << " " << obj.canMoveThrough << " "
			<< obj.id << "\n";

	txt.close();
}

void Editor::remove_object()
{
	int x = 0, y = 0;
	SDL_GetMouseState(&x, &y);
	x += wptr->camera.x;
	y += wptr->camera.y;

	int i = 0;
	for (const Object& obj : m_.objects)
	{
		if ((x > obj.box.x) && (x < obj.box.x + obj.box.w) && (y > obj.box.y) && (y < obj.box.y + obj.box.h))
		{
			m_.objects.erase(m_.objects.begin() + i);
			break;
		}
		i++;
	}
}

void Editor::process_events()
{
	while (SDL_PollEvent(&wptr->e) != 0)
	{
		if (wptr->e.type == SDL_QUIT)
			quit_ = true;

		if (wptr->e.type == SDL_KEYDOWN)
		{
			switch (wptr->e.key.keysym.sym)
			{
			case SDLK_m:
				{
					if (mode_ == TILES_EDITOR)
					{
						mode_ = OBJECT_EDITOR;
						if (editor_move_through_)
							SDL_SetWindowTitle(wptr->window, "Level Designer. Can move through object");
						else
							SDL_SetWindowTitle(wptr->window, "Level Designer. Can't move through object");
					}
					else
					{
						mode_ = TILES_EDITOR;
						std::stringstream title;
						title << "Level Designer. Current Tile ID: " << current_type_;
						SDL_SetWindowTitle(wptr->window, title.str().c_str());
					}
					break;
				}
			case SDLK_b:
				{
					if (mode_ == OBJECT_EDITOR)
					{
						if (editor_move_through_)
						{
							SDL_SetWindowTitle(wptr->window, "Level Designer. Can't move through object");
							editor_move_through_ = false;
						}
						else
						{
							SDL_SetWindowTitle(wptr->window, "Level Designer. Can move through object");
							editor_move_through_ = true;
						}
					}
					break;
				}
			default:
				break;
			}
		}
		if (wptr->e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (wptr->e.button.button == SDL_BUTTON_LEFT)
			{
				if (mode_ == TILES_EDITOR)
					put_tile();
				else
					put_object();
			}
			else if (wptr->e.button.button == SDL_BUTTON_RIGHT)
			{
				if (mode_ == OBJECT_EDITOR)
					remove_object();
				else
				{
					if (wptr->e.type == SDL_MOUSEBUTTONDOWN)
					{
						int x = 0, y = 0;
						SDL_GetMouseState(&x, &y);
						select_.x = x;
						select_.y = y;
						right_click_ = true;
					}
				}
			}
		}

		if (wptr->e.type == SDL_MOUSEBUTTONUP)
		{
			if (wptr->e.button.button == SDL_BUTTON_RIGHT)
			{
				if (mode_ == TILES_EDITOR)
				{
					if (wptr->e.type == SDL_MOUSEBUTTONUP)
					{
						right_click_ = false;
						replace_tiles();
					}
				}
			}
		}

		if (wptr->e.type == SDL_MOUSEWHEEL)
		{
			if (wptr->e.wheel.y > 0) // scroll up
			{
				if (current_type_ + 1 < m_.totalTileSetTiles && mode_ == TILES_EDITOR)
				{
					current_type_++;
					std::stringstream title;
					title << "Level Designer. Current Tile ID: " << current_type_;
					SDL_SetWindowTitle(wptr->window, title.str().c_str());
				}
				else if (current_type_object_ + 1 < m_.objCount && mode_ == OBJECT_EDITOR)
					current_type_object_++;
			}
			else if (wptr->e.wheel.y < 0) // scroll down
			{
				if (current_type_ >= 1 && mode_ == TILES_EDITOR)
				{
					current_type_--;
					std::stringstream title;
					title << "Level Designer. Current Tile ID: " << current_type_;
					SDL_SetWindowTitle(wptr->window, title.str().c_str());
				}
				else if (current_type_object_ >= 1 && mode_ == OBJECT_EDITOR)
					current_type_object_--;
			}
		}
	}
}

void Editor::replace_tiles()
{
	if (select_.x + select_.w < select_.x)
	{
		select_.x = select_.x + select_.w;
		select_.w = -select_.w;
	}

	if (select_.y + select_.h < select_.y)
	{
		select_.y = select_.y + select_.h;
		select_.h = -select_.h;
	}

	select_.x += wptr->camera.x;
	select_.y += wptr->camera.y;

	for (int i = 0; i < m_.totalTiles; i++)
	{
		if (check_collision(select_, m_.tileSet[i].box))
			m_.tileSet[i].set_tile(m_.tileSet[i].box.x, m_.tileSet[i].box.y, current_type_, m_.tileInfo[current_type_]);
	}
}

Editor* eptr;
