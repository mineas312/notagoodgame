#pragma once
#include "stdafx.h"
#include <glad/glad.h>
#include <SDL_opengl.h>
#define GLM_FORCE_SSE3
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define KEY_COLOR_R 0
#define KEY_COLOR_G 0xFF
#define KEY_COLOR_B 0xFF

class Object
{
public:
	Object() noexcept : width{ 0.0f }, height{ 0.0f }, texWidth{ 0 }, texHeight{ 0 }, rangePerWidthTex{ 0.0f }, rangePerHeightTex{ 0.0f }
	{}

	void render(int x, int y);

	void setObject(const char * path, SDL_Rect * clip = NULL);

private:
	bool loadTexture(const char * path);

	void free();

public:
	int texWidth;
	int texHeight;
private:
	GLuint indexData[6] = { 0, 1, 2, 1, 2, 3 };
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint texture;
	float width;
	float height;
	float rangePerWidthTex;
	float rangePerHeightTex;
};
