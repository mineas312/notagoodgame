#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

extern TTF_Font* font;
extern SDL_Renderer* g_renderer;

bool check_collision(const SDL_Rect& a, const SDL_Rect& b);

class Texture
{
public:
	Texture() = default;

	bool load_texture(const char* path);

	bool loadFromRenderedText(char* textureText, SDL_Color textColor);

	void free();

	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) const;

public:
	SDL_Texture* texture = nullptr;
	int width = 0;
	int height = 0;
};