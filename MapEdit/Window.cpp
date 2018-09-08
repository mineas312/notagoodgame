#include "Window.h"
#include "Texture.h"
#include "Map.h"

Window::Window(const int width, const int height) : e(), camera(), SCREEN_WIDTH {width}, SCREEN_HEIGHT {height}
{
}

bool Window::init()
{
	//Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	window = SDL_CreateWindow("Level Designer. Current Tile ID: 0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		return false;
	}

	g_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_renderer == nullptr)
	{
		printf("Cannot create renderer");
		return false;
	}

	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
	camera = { 0, 0, SCREEN_WIDTH - 256, SCREEN_HEIGHT };

	return true;
}

void Window::set_camera(int mapWidth, int mapHeight, Uint32 fps)
{
	if (fps == 0)
		return;

	//Mouse offsets
	int x = 0, y = 0;

	//Get mouse offsets
	SDL_GetMouseState(&x, &y);

	//Move camera to the left if needed
	if (x < TILE_WIDTH)
	{
		camera.x -= 800 / fps;
	}

	//Move camera to the right if needed
	if ((x > SCREEN_WIDTH - TILE_WIDTH - 256) && (x < SCREEN_WIDTH - 256))
	{
		camera.x += 800 / fps;
	}

	//Move camera up if needed
	if (y < TILE_WIDTH)
	{
		camera.y -= 800 / fps;
	}

	//Move camera down if needed
	if (y > SCREEN_HEIGHT - TILE_WIDTH)
	{
		camera.y += 800 / fps;
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

	if (camera.x > mapWidth - camera.w)
	{
		camera.x = mapWidth - camera.w;
	}

	if (camera.y > mapHeight - camera.h)
	{
		camera.y = mapHeight - camera.h;
	}
}

Window * wptr;
