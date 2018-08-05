#include "stdafx.h"
#include "Window.h"
#include "Texture.h"
#include "Shader.h"
#include "Common.h"
#include "Camera.h"
#include "Game.h"

#include <gli/image.hpp>
#include <gli/load_ktx.hpp>

void Texture::render(int x, int y)
{
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texture);

	const glm::mat4 model = glm::translate(identityMatrix, glm::vec3(x * winptr->rangePerWidthPixel, -y * winptr->rangePerHeightPixel, 0.0f));

	const glm::mat4 mvp = camptr->proj * camptr->view * model;

	glBindBuffer(GL_UNIFORM_BUFFER, gptr->ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(mvp));
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, gptr->ubo);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
}

void Texture::setTexture(const char * path, SDL_Rect * clip, int w, int h)
{
	loadTexture(path);

	width = texWidth * winptr->rangePerWidthPixel;
	height = texHeight * winptr->rangePerHeightPixel;

	rangePerWidthTex = 1.0f / texWidth;
	rangePerHeightTex = 1.0f / texHeight;

	float * vertices = nullptr;

	if (clip != NULL)
	{
		float x = clip->x * rangePerWidthTex;
		float y = clip->y * rangePerHeightTex;
		float w = clip->w * rangePerWidthTex;
		float h = clip->h * rangePerHeightTex;

		float cWidth = clip->w * winptr->rangePerWidthPixel;
		float cHeight = clip->h * winptr->rangePerHeightPixel;

		vertices = new float[20]{
			-1.0f, 1.0f, 0.0f, x, 1.0f - y,
			-1.0f + cWidth, 1.0f, 0.0f, x + w, 1.0f - y,
			-1.0f, 1.0f - cHeight, 0.0f, x, 1.0f - y - h,
			-1.0f + cWidth, 1.0f - cHeight, 0.0f, x + w, 1.0f - y - h
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint8_t), indexData, GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(shadptr->ProgID);
	glUniform1i(glGetUniformLocation(shadptr->ProgID, "sampler"), 0);
}

bool Texture::loadTexture(const char * path)
{
	free();
	int components;
	GLuint texID;

#ifdef USE_FORMAT_PNG
	stbi_set_flip_vertically_on_load(true);
	unsigned char * img = stbi_load(path, &texWidth, &texHeight, &components, 4);

	if (img == NULL)
		return false;
#else
	gli::texture img = gli::load_ktx(path);

	texWidth = img.extent().x;
	texHeight = img.extent().y;

	if (img.data() == NULL)
		return false;
#endif

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

#ifdef USE_FORMAT_PNG
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
#else 
	glCompressedTexImage2D(GL_TEXTURE_2D, img.base_level(), GL_COMPRESSED_RGBA_BPTC_UNORM, img.extent().x, img.extent().y, 0, img.size(), img.data());
	printf("%d", glGetError());
#endif

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
#ifdef USE_FORMAT_PNG
	delete[] img;
#endif
	texture = texID;
}

void Texture::free()
{
	glDeleteTextures(1, &texture);
}