#include "stdafx.h"
#include "Window.h"
#include "Object.h"
#include "Shader.h"
#include <stb_image.h>

void Object::render(int x, int y)
{
	glBindVertexArray(vao);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * winptr->rangePerWidthPixel, -y * winptr->rangePerHeightPixel, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shadptr->ProgID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

void Object::setObject(const char * path, SDL_Rect * clip)
{
	loadTexture(path);

	width = texWidth * winptr->rangePerWidthPixel;
	height = texHeight * winptr->rangePerHeightPixel;

	rangePerWidthTex = 1.0f / texWidth;
	rangePerHeightTex = 1.0f / texHeight;

	float * vertices;

	if (clip != NULL)
	{
		vertices = new float[20]{
			-1.0f, 1.0f, 0.0f, clip->x * rangePerWidthTex, 1.0f - clip->y * rangePerHeightTex,// tl
			-1.0f + width, 1.0f, 0.0f, (clip->x * rangePerWidthTex) + (clip->w * rangePerWidthTex), 1.0f - clip->y * rangePerHeightTex,// tr
			-1.0f, 1.0f - height, 0.0f, clip->x * rangePerWidthTex, 1.0f - (clip->y * rangePerHeightTex) - (clip->h * rangePerHeightTex),	// bl
			-1.0f + width, 1.0f - height, 0.0f, (clip->x * rangePerWidthTex) + (clip->w * rangePerWidthTex), 1.0f - (clip->y * rangePerHeightTex) - (clip->h * rangePerHeightTex) // br
		};
	}
	else
	{
		vertices = new float[20]{
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f + width, 1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, 1.0f - height, 0.0f, 0.0f, 0.0f,
			-1.0f + width, 1.0f - height, 0.0f, 1.0f, 0.0f
		};
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, texture);
	// TODO: Tu (przy uniformie) wywala b³¹d ¿e nie ma zbindowanego shader programu. WeŸ to sprawdŸ
	glUniform1i(glGetUniformLocation(shadptr->ProgID, "sampler"), 0);
}

bool Object::loadTexture(const char * path)
{
	free();
	int components;
	GLuint texID;

	stbi_set_flip_vertically_on_load(true);
	unsigned char * data = stbi_load(path, &texWidth, &texHeight, &components, 4);
	if (data != NULL)
	{
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		if (components == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (components == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		delete[] data;
		texture = texID;
		return true;
	}
	return false;
}

void Object::free()
{
	glDeleteTextures(1, &texture);
}
