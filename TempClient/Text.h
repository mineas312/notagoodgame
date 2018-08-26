#pragma once
#include "Window.h"
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Charact
{
	GLuint textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

class Text
{
public:
	Text();
	~Text();
	void init();
	// -Set width to 0 if you want to dynamically calculate width based on the given height
	void setSize(int width, int height);
	void render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
private:
	void loadCharacterSet();
private:
	FT_Library ft;
	FT_Face face;

	glm::mat4 projection;
	GLuint vao, vbo;

	std::map<GLchar, Charact> characters;
};
extern Text * tptr;