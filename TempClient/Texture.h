#pragma once

#define KEY_COLOR_R 0
#define KEY_COLOR_G 0xFF
#define KEY_COLOR_B 0xFF

class Texture
{
public:
	Texture() noexcept : texture{ NULL }, width{ 0 }, height{ 0 }
	{}

	bool loadTexture(const char* path, SDL_Renderer* renderer);

	void free();

	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();

	int getHeight();

private:
	SDL_Texture* texture;
	int width;
	int height;
};
