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
	void renderGUI(Map &m, int currentID, int mode, int currentObjID)
	{
		SDL_SetRenderDrawColor(g_renderer, 100, 100, 100, 255);
		SDL_RenderFillRect(g_renderer, &backgroudGUI);

		if (mode == 0)
		{
			for (int i = 0; i < m.totalTileSetTiles; i++)
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
		else
		{
			for (int i = 0; i < m.objCount; i++)
			{
				g_ObjectTextures[i].render(objectsPosition[i].x + wptr->camera.w, objectsPosition[i].y);
				if (currentObjID == i)
				{
					SDL_SetRenderDrawColor(g_renderer, 150, 0, 0, 255);
					SDL_Rect tmpRect = objectsPosition[i];
					tmpRect.x += wptr->camera.w;
					SDL_RenderDrawRect(g_renderer, &tmpRect);
				}
			}
		}
	}
	void init(Map &m)
	{
		tilesPosition = new SDL_Rect[m.totalTileSetTiles];
		objectsPosition = new SDL_Rect[m.objCount];

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
		int x = 0, y = 0, maxY = 0;
		for (int i = 0; i < m.objCount; i++)
		{
			objectsPosition[i].w = g_ObjectTextures[i].width;
			objectsPosition[i].h = g_ObjectTextures[i].height;
			if (objectsPosition[i].h > maxY)
				maxY = g_ObjectTextures[i].height;
			x += g_ObjectTextures[i].width;
			while (x >= 160)
			{
				x = 0;
				y += maxY;
			}
			objectsPosition[i].x = x + 50;
			objectsPosition[i].y = y;
		}
	}
	void selectRect(SDL_Rect * rect)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		rect->w = x - rect->x;
		rect->h = y - rect->y;
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 200, 100);
		SDL_RenderFillRect(g_renderer, rect);
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 150, 255);
		SDL_RenderDrawRect(g_renderer, rect);
	}
private:
	SDL_Rect backgroudGUI;
	SDL_Rect * tilesPosition;
	SDL_Rect * objectsPosition;
};

GUI * gptr;