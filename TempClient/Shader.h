#pragma once
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <sstream>

class Shader
{
public:
	bool isValid = false;

	Shader() noexcept;
	Shader(const char* vs, const char* fs);

	bool InitShader(const char* vs, const char* fs);

private:
	GLuint VSID;
	GLuint FSID;
	GLuint ProgID;

	std::stringstream vsCode;
	std::stringstream fsCode;

	void Log(GLuint ID);
};
