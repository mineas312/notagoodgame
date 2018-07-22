#pragma once
#include "stdafx.h"

#define KEY_COLOR_R 0
#define KEY_COLOR_G 0xFF
#define KEY_COLOR_B 0xFF

class Texture
{
public:
	Texture() noexcept : width{ 0.0f }, height{ 0.0f }, texWidth{ 0 }, texHeight{ 0 }, rangePerWidthTex{ 0.0f }, rangePerHeightTex{ 0.0f }
	{}

	void render(int x, int y);

	void setTexture(const char * path, SDL_Rect * clip = NULL, int w = 0, int h = 0);

private:
	bool loadTexture(const char * path);

	void free();

public:
	int texWidth;
	int texHeight;
	GLuint vao;
	GLuint texture;
private:
	const GLuint indexData[6] = { 0, 1, 2, 1, 2, 3 };
	GLuint vbo;
	GLuint ibo;
	float width;
	float height;
	float rangePerWidthTex;
	float rangePerHeightTex;
};
