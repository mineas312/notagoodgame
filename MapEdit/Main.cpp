#define TILE_WIDTH 80
#define TILE_HEIGHT 80

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <fstream>

enum TileEnum
{
	TILE_RED,
	TILE_GREEN,
	TILE_BLUE,
	TILE_CENTER,
	TILE_TOP,
	TILE_TOPRIGHT,
	TILE_RIGHT,
	TILE_BOTTOMRIGHT,
	TILE_BOTTOM,
	TILE_BOTTOMLEFT,
	TILE_LEFT,
	TILE_TOPLEFT,
	TOTAL_TILES,
	NO_TILE
};

class Map
{
public:
	Map()
	{
		MAP_WIDTH = 0;
		MAP_HEIGHT = 0;
		TOTAL_TILES = 0;
	}
	void setMap(int width, int height)
	{
		MAP_WIDTH = width;
		MAP_HEIGHT = height;
		TOTAL_TILES = width / TILE_WIDTH * height / TILE_HEIGHT;
	}
	int getWidth()
	{
		return MAP_WIDTH;
	}
	int getHeight()
	{
		return MAP_HEIGHT;
	}
	int getTotalTiles()
	{
		return Map::TOTAL_TILES;
	}
private:
	int MAP_WIDTH;
	int MAP_HEIGHT;
	int TOTAL_TILES;
};

class Tile
{
public:
	Tile()
	{

	}
	Tile(int x, int y, TileEnum tileType)
	{
		//Get the offsets
		box.x = x;
		box.y = y;

		//Set the collision box
		box.w = TILE_WIDTH;
		box.h = TILE_HEIGHT;

		//Get the tile type
		type = tileType;
	}
	void show(SDL_Rect camera, SDL_Surface * tileSheet, SDL_Surface * screen, SDL_Rect * clips)
	{
		//If the tile is on screen
		if (check_collision(camera, box) == true)
		{
			//Show the tile
			apply_surface(box.x - camera.x, box.y - camera.y, tileSheet, screen, &clips[type]);
		}
	}
	void setTile(int x, int y, TileEnum tileType)
	{
		//Get the offsets
		box.x = x;
		box.y = y;

		//Set the collision box
		box.w = TILE_WIDTH;
		box.h = TILE_HEIGHT;

		//Get the tile type
		type = tileType;
	}
	TileEnum get_type()
	{
		return type;
	}
	SDL_Rect &get_box()
	{
		return box;
	}
	bool check_collision(SDL_Rect &A, SDL_Rect &B)
	{
		//The sides of the rectangles
		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;

		//Calculate the sides of rect A
		leftA = A.x;
		rightA = A.x + A.w;
		topA = A.y;
		bottomA = A.y + A.h;

		//Calculate the sides of rect B
		leftB = B.x;
		rightB = B.x + B.w;
		topB = B.y;
		bottomB = B.y + B.h;

		//If any of the sides from A are outside of B
		if (bottomA <= topB)
		{
			return false;
		}

		if (topA >= bottomB)
		{
			return false;
		}

		if (rightA <= leftB)
		{
			return false;
		}

		if (leftA >= rightB)
		{
			return false;
		}

		//If none of the sides from A are outside B
		return true;
	}
	void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
	{
		//Holds offsets
		SDL_Rect offset;

		//Get offsets
		offset.x = x;
		offset.y = y;

		//Blit
		SDL_BlitSurface(source, clip, destination, &offset);
	}
private:
	SDL_Rect box;
	TileEnum type;
};

class Timer
{
public:
	Timer()
	{
		//Initialize the variables
		startTicks = 0;
		pausedTicks = 0;
		paused = false;
		started = false;
	}
	void start()
	{
		//Start the timer
		started = true;

		//Unpause the timer
		paused = false;

		//Get the current clock time
		startTicks = SDL_GetTicks();
	}
	void stop()
	{
		//Stop the timer
		started = false;

		//Unpause the timer
		paused = false;
	}
	void pause()
	{
		//If the timer is running and isn't already paused
		if ((started == true) && (paused == false))
		{
			//Pause the timer
			paused = true;

			//Calculate the paused ticks
			pausedTicks = SDL_GetTicks() - startTicks;
		}
	}
	void unpause()
	{
		//If the timer is paused
		if (paused == true)
		{
			//Unpause the timer
			paused = false;

			//Reset the starting ticks
			startTicks = SDL_GetTicks() - pausedTicks;

			//Reset the paused ticks
			pausedTicks = 0;
		}
	}
	int get_ticks()
	{
		//If the timer is running
		if (started == true)
		{
			//If the timer is paused
			if (paused == true)
			{
				//Return the number of ticks when the the timer was paused
				return pausedTicks;
			}
			else
			{
				//Return the current time minus the start time
				return SDL_GetTicks() - startTicks;
			}
		}

		//If the timer isn't running
		return 0;
	}
	bool is_started()
	{
		return started;
	}
	bool is_paused()
	{
		return paused;
	}
private:
	int startTicks;
	int pausedTicks;
	bool paused;
	bool started;
};

class Window
{
public:
	Window()
	{
		window = NULL;
		screen = NULL;
		SCREEN_WIDTH = 640;
		SCREEN_HEIGHT = 480;
		currentType = TILE_RED;
		tiles = NULL;
		tileSheet = NULL;
		camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		quit = false;
	}
	bool init()
	{
		//Initialize all SDL subsystems
		if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		{
			return false;
		}
		window = SDL_CreateWindow("Level Designer. Current Tile: Red Tile", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			return false;
		}

		screen = SDL_GetWindowSurface(window);

		m.setMap(1280, 960);

		tiles = new Tile[m.getTotalTiles()];
		//If everything initialized fine
		return true;
	}
	void show_type(int tileType)
	{
		switch (tileType)
		{
		case TILE_RED:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Red Floor");
			break;

		case TILE_GREEN:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Green Floor");
			break;

		case TILE_BLUE:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Blue Floor");
			break;

		case TILE_CENTER:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Center Wall");
			break;

		case TILE_TOP:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Top Wall");
			break;

		case TILE_TOPRIGHT:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Top Right Wall");
			break;

		case TILE_RIGHT:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Right Wall");
			break;

		case TILE_BOTTOMRIGHT:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Bottom Right Wall");
			break;

		case TILE_BOTTOM:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Bottom Wall");
			break;

		case TILE_BOTTOMLEFT:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Bottom Left Wall");
			break;

		case TILE_LEFT:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Left Wall");
			break;

		case TILE_TOPLEFT:
			SDL_SetWindowTitle(window, "Level Designer. Current Tile: Top Left Wall");
			break;
		};
	}
	bool load_files()
	{
		//Load the tile sheet
		tileSheet = load_image("tiles.png");

		//If there was a problem in loading the tiles
		if (tileSheet == NULL)
		{
			return false;
		}

		//If everything loaded fine
		return true;
	}
	void clean_up()
	{
		//Free the surface
		SDL_FreeSurface(tileSheet);

		//Free the tiles
		delete[] tiles;

		//Quit SDL
		SDL_Quit();
	}
	void set_camera()
	{
		//Mouse offsets
		int x = 0, y = 0;

		//Get mouse offsets
		SDL_GetMouseState(&x, &y);

		//Move camera to the left if needed
		if (x < TILE_WIDTH)
		{
			camera.x -= 20;
		}

		//Move camera to the right if needed
		if (x > SCREEN_WIDTH - TILE_WIDTH)
		{
			camera.x += 20;
		}

		//Move camera up if needed
		if (y < TILE_WIDTH)
		{
			camera.y -= 20;
		}

		//Move camera down if needed
		if (y > SCREEN_HEIGHT - TILE_WIDTH)
		{
			camera.y += 20;
		}

		//Keep the camera in bounds.
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		if (camera.x > m.getWidth() - camera.w)
		{
			camera.x = m.getWidth() - camera.w;
		}
		if (camera.y > m.getHeight() - camera.h)
		{
			camera.y = m.getHeight() - camera.h;
		}
	}
	void put_tile(int tileType)
	{
		//Mouse offsets
		int x = 0, y = 0;

		//Get mouse offsets
		SDL_GetMouseState(&x, &y);

		//Adjust to camera
		x += camera.x;
		y += camera.y;

		//Go through tiles
		for (int t = 0; t < m.getTotalTiles(); t++)
		{
			//Get tile's collision box
			SDL_Rect box = tiles[t].get_box();

			//If the mouse is inside the tile
			if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
			{
				//Replace it with new one
				tiles[t].setTile(box.x, box.y, TileEnum(tileType));
			}
		}
	}
	void clip_tiles()
	{
		//Clip the sprite sheet
		clips[TILE_RED].x = 0;
		clips[TILE_RED].y = 0;
		clips[TILE_RED].w = TILE_WIDTH;
		clips[TILE_RED].h = TILE_HEIGHT;

		clips[TILE_GREEN].x = 0;
		clips[TILE_GREEN].y = 80;
		clips[TILE_GREEN].w = TILE_WIDTH;
		clips[TILE_GREEN].h = TILE_HEIGHT;

		clips[TILE_BLUE].x = 0;
		clips[TILE_BLUE].y = 160;
		clips[TILE_BLUE].w = TILE_WIDTH;
		clips[TILE_BLUE].h = TILE_HEIGHT;

		clips[TILE_TOPLEFT].x = 80;
		clips[TILE_TOPLEFT].y = 0;
		clips[TILE_TOPLEFT].w = TILE_WIDTH;
		clips[TILE_TOPLEFT].h = TILE_HEIGHT;

		clips[TILE_LEFT].x = 80;
		clips[TILE_LEFT].y = 80;
		clips[TILE_LEFT].w = TILE_WIDTH;
		clips[TILE_LEFT].h = TILE_HEIGHT;

		clips[TILE_BOTTOMLEFT].x = 80;
		clips[TILE_BOTTOMLEFT].y = 160;
		clips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
		clips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

		clips[TILE_TOP].x = 160;
		clips[TILE_TOP].y = 0;
		clips[TILE_TOP].w = TILE_WIDTH;
		clips[TILE_TOP].h = TILE_HEIGHT;

		clips[TILE_CENTER].x = 160;
		clips[TILE_CENTER].y = 80;
		clips[TILE_CENTER].w = TILE_WIDTH;
		clips[TILE_CENTER].h = TILE_HEIGHT;

		clips[TILE_BOTTOM].x = 160;
		clips[TILE_BOTTOM].y = 160;
		clips[TILE_BOTTOM].w = TILE_WIDTH;
		clips[TILE_BOTTOM].h = TILE_HEIGHT;

		clips[TILE_TOPRIGHT].x = 240;
		clips[TILE_TOPRIGHT].y = 0;
		clips[TILE_TOPRIGHT].w = TILE_WIDTH;
		clips[TILE_TOPRIGHT].h = TILE_HEIGHT;

		clips[TILE_RIGHT].x = 240;
		clips[TILE_RIGHT].y = 80;
		clips[TILE_RIGHT].w = TILE_WIDTH;
		clips[TILE_RIGHT].h = TILE_HEIGHT;

		clips[TILE_BOTTOMRIGHT].x = 240;
		clips[TILE_BOTTOMRIGHT].y = 160;
		clips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
		clips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
	}
	bool set_tiles()
	{
		//The tile offsets
		int x = 0, y = 0;

		//Open the map
		std::ifstream map("lazy.map");

		//If the map couldn't be loaded
		if (!map.is_open())
		{
			//Initialize the tiles
			for (int t = 0; t < m.getTotalTiles(); t++)
			{
				//Put a floor tile
				tiles[t].setTile(x, y, TileEnum(t % (TILE_BLUE + 1)));

				//Move to next tile spot
				x += TILE_WIDTH;

				//If we've gone too far
				if (x >= m.getWidth())
				{
					//Move back
					x = 0;

					//Move to the next row
					y += TILE_HEIGHT;
				}
			}
		}
		else
		{
			//Initialize the tiles
			for (int t = 0; t < m.getTotalTiles(); t++)
			{
				//Determines what kind of tile will be made
				int tileType = -1;

				//Read tile from map file
				map >> tileType;

				//If there was a problem in reading the map
				if (map.fail() == true)
				{
					//Stop loading map
					map.close();
					return false;
				}

				//If the number is a valid tile number
				if ((tileType >= 0) && (tileType < TileEnum::TOTAL_TILES))
				{
					tiles[t].setTile(x, y, TileEnum(tileType));
				}
				//If we don't recognize the tile type
				else
				{
					//Stop loading map
					map.close();
					return false;
				}

				//Move to next tile spot
				x += TILE_WIDTH;

				//If we've gone too far
				if (x >= m.getWidth())
				{
					//Move back
					x = 0;

					//Move to the next row
					y += TILE_HEIGHT;
				}
			}

			//Close the file
			map.close();
		}

		return true;
	}
	void save_tiles()
	{
		//Open the map
		std::ofstream map("lazy.map");

		//Go through the tiles
		for (int t = 0; t < m.getTotalTiles(); t++)
		{
			//Write tile type to file
			map << tiles[t].get_type() << " ";
		}

		//Close the file
		map.close();
	}
	SDL_Window * getWindow()
	{
		return window;
	}
	void loop()
	{
		const int FRAMES_PER_SECOND = 20;

		//Clip the tile sheet
		clip_tiles();

		//Set the tiles
		if (set_tiles() == false)
		{
			exit(-3);
		}

		//While the user hasn't quit
		while (quit == false)
		{
			//Start the frame timer
			fps.start();

			//While there's events to handle
			while (SDL_PollEvent(&e))
			{
				//When the user clicks
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					//On left mouse click
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						//Put the tile
						put_tile(currentType);
					}
				}

				if (e.type == SDL_MOUSEWHEEL)
				{
					if (e.wheel.y > 0)
					{
						//Scroll through tiles
						currentType--;

						if (currentType < TILE_RED)
						{
							currentType = TILE_TOPLEFT;
						}

						//Show the current tile type
						show_type(currentType);
					}
					else if (e.wheel.y < 0)
					{
						//Scroll through tiles
						currentType++;

						if (currentType > TILE_TOPLEFT)
						{
							currentType = TILE_RED;
						}

						//Show the current tile type
						show_type(currentType);
					}
				}

				//If the user has Xed out the window
				if (e.type == SDL_QUIT)
				{
					//Quit the program
					quit = true;
				}
			}

			//Set the camera
			set_camera();

			//Show the tiles
			for (int t = 0; t < m.getTotalTiles(); t++)
			{
				tiles[t].show(camera, tileSheet, screen, clips);
			}

			SDL_UpdateWindowSurface(window);

			//Cap the frame rate
			if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
			{
				SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
			}
		}

		//Save the tile map
		save_tiles();

		//Clean up
		clean_up();
	}
	SDL_Surface *load_image(std::string filename)
	{
		//The image that's loaded
		SDL_Surface* loadedImage = NULL;

		//The optimized surface that will be used
		SDL_Surface* optimizedImage = NULL;

		//Load the image
		loadedImage = IMG_Load(filename.c_str());

		//If the image loaded
		if (loadedImage != NULL)
		{
			//Create an optimized surface
			optimizedImage = SDL_ConvertSurface(loadedImage, loadedImage->format, NULL);

			//Free the old surface
			SDL_FreeSurface(loadedImage);

			//If the surface was optimized
			if (optimizedImage != NULL)
			{
				//Color key surface
				SDL_SetColorKey(optimizedImage, SDL_TRUE, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
			}
		}

		//Return the optimized surface
		return optimizedImage;
	}
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	SDL_Window * window;
	SDL_Surface * screen;
	SDL_Rect camera;
	SDL_Surface *tileSheet;
	Map m;
	SDL_Rect clips[TOTAL_TILES];
	bool quit;
	Tile * tiles;
	int currentType;
	Timer fps;
public:
	SDL_Event e;
};

int main(int argc, char* args[])
{
	Window w;

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
