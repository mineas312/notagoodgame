#pragma once

#include "Map.h"

class Window
{
public:
	// -Sets width and height of window
	Window(const int width, const int height) noexcept : SCREEN_WIDTH{ width }, SCREEN_HEIGHT{ height }, window{ NULL }, context{ NULL }, camera{ NULL }, rangePerWidthPixel { 2.0f / SCREEN_WIDTH }, rangePerHeightPixel { 2.0f / SCREEN_HEIGHT }
	{}

	// -Initializes window, renderer
	void init();

	void centerCamera(const int x, const int y, const Map& map) noexcept;

	void close() noexcept;

private:
	void setOpenGLAtrributes() noexcept;

public:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	float rangePerWidthPixel;
	float rangePerHeightPixel;

	SDL_Window * window;
	SDL_Rect camera;
	SDL_GLContext context;

	GLint vertexPos2DLocation = -1;
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;

};

extern Window* winptr;
