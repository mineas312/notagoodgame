#pragma once
#include "Map.h"

class GUI
{
public:
	GUI()
	{
		backgroudGUI.x = wptr->SCREEN_WIDTH - 256;
		backgroudGUI.y = 0;
		backgroudGUI.w = 256;
		backgroudGUI.h = wptr->SCREEN_HEIGHT;
	}
	void renderGUI(Map &m, int currentID)
	{
		SDL_SetRenderDrawColor(g_renderer, 100, 100, 100, 255);
		SDL_RenderFillRect(g_renderer, &backgroudGUI);

		for(int i = 0; i < m.totalTileSetTiles; i++)
		{
			g_TileSetTiles.render(tilesPosition[i].x + wptr->camera.w, tilesPosition[i].y, &m.tilesPlace[i]);
			if (currentID == i)
			{
				SDL_SetRenderDrawColor(g_renderer, 150, 0, 0, 255);
				SDL_Rect tmpRect = tilesPosition[i];
				tmpRect.x += wptr->camera.w;
				while (tmpRect.y >= wptr->SCREEN_HEIGHT)
				{
					for (int i = 0; i < m.totalTileSetTiles; i++)
					{
						tilesPosition[i].y -= TILE_HEIGHT;
					}
				}
				while (tmpRect.y < 0)
				{
					for (int i = 0; i < m.totalTileSetTiles; i++)
					{
						tilesPosition[i].y += TILE_HEIGHT;
					}
				}
				SDL_RenderDrawRect(g_renderer, &tmpRect);
			}
		}
	}
	void init(Map &m)
	{
		tilesPosition = new SDL_Rect[m.totalTileSetTiles];

		for (int i = 0; i < m.totalTileSetTiles; i++)
		{
			tilesPosition[i].w = TILE_WIDTH;
			tilesPosition[i].h = TILE_HEIGHT;
			int x = 0, y = 0;
			x = i * TILE_WIDTH;
			while (x >= 160)
			{
				x -= 160;
				y += 32;
			}
			tilesPosition[i].x = x + 50;
			tilesPosition[i].y = y;
		}
	}
private:
	SDL_Rect backgroudGUI;
	SDL_Rect * tilesPosition;
};

GUI * gptr;