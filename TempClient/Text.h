#pragma once
#include "Window.h"
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Charact
{
	float sizex, sizey;
	float bl, bt;
	float ax, ay;
	GLfloat tx;
};

class Text
{
public:
	Text();
	~Text();
	void init();
	// -Set width to 0 if you want to dynamically calculate width based on the given height
	void render(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3& color);
private:
	void createTextureAtlas();
private:
	FT_Library ft;
	FT_Face face;

	glm::mat4 projection;
	GLuint vao, vbo;
	GLuint tex;

	Charact characters[128];

	int wAtlas = 0;
	int hAtlas = 0;
};
extern Text * tptr;