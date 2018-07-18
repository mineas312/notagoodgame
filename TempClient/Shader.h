#pragma once
#include <glad/glad.h>
#include <SDL_opengl.h>

class Shader
{
public:
	bool isValid = false;

	Shader() noexcept;
	Shader(const char* vs, const char* fs);

	bool InitShader(const char* vs, const char* fs);

public:
	GLuint ProgID;

private:
	GLuint VSID;
	GLuint FSID;

	void Log(GLuint ID);
};

extern Shader * shadptr;