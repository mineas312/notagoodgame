#pragma once
#include <glad/glad.h>
#include <SDL_opengl.h>

class Shader
{
public:
	Shader()
	{}

	void init();

private:
	void Log(GLuint ID);
	void initShader(const char* __restrict vs, const char* __restrict fs, GLuint &program);

public:
	GLuint progGraphics;
	GLuint progText;
};

extern Shader * shadptr;