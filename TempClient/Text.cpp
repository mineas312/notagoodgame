#include "stdafx.h"
#include "Text.h"


Text::Text()
{
}


Text::~Text()
{
}

void Text::init()
{
	if (FT_Init_FreeType(&ft))
	{
		printf("[ERROR] FreeType could not be initialized!");
		exit(-3);
	}
	if (FT_New_Face(ft, "res/font/arial.ttf", 0, &face))
	{
		printf("[ERROR] Failed to load font!");
		exit(-4);
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

	createTextureAtlas();
}

void Text::render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);
	glUniform3f(glGetUniformLocation(shadptr->progText, "textColor"), color.x, color.y, color.z);

	GLfloat(*verts)[6][4] = new GLfloat[text.size()][6][4];

	std::string::const_iterator c;
	int n = 0;
	for (c = text.begin(); c != text.end(); c++)
	{
		Charact ch = characters[*c];

		GLfloat xpos = x + ch.bl * scale;
		GLfloat ypos = y - (ch.sizey - ch.bt) * scale;

		GLfloat w = ch.sizex * scale;
		GLfloat h = ch.sizey * scale;

		GLfloat vertices[6][4]{
			{ xpos,     ypos + h,   ch.tx, 0.0 },
			{ xpos,     ypos,       ch.tx, 1.0 - (1.0 - ch.sizey / hAtlas) },
			{ xpos + w, ypos,       ch.tx + ch.sizex / wAtlas, 1.0 - (1.0 - ch.sizey / hAtlas) },

			{ xpos,     ypos + h,   ch.tx, 0.0 },
			{ xpos + w, ypos,       ch.tx + ch.sizex / wAtlas, 1.0 - (1.0 - ch.sizey / hAtlas) },
			{ xpos + w, ypos + h,   ch.tx + ch.sizex / wAtlas, 0.0 }
		};
		
		for (int i = 0; i < 6; i++)
		{
			for (int l = 0; l < 4; l++)
				verts[n][i][l] = vertices[i][l];
		}

		x += ((GLuint)ch.ax >> 6) * scale;
		y += ((GLuint)ch.ay >> 6) * scale;
		n++;
	}
	glBindTexture(GL_TEXTURE_2D, tex);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat[6][4]) * text.size(), verts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4 * n, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6 * 4 * n, verts);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, n * 6);

	delete[] verts;
}

void Text::createTextureAtlas()
{
	glUseProgram(shadptr->progText);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	projection = glm::ortho(0.0f, (float)winptr->SCREEN_WIDTH, 0.0f, (float)winptr->SCREEN_HEIGHT);
	glUniformMatrix4fv(glGetUniformLocation(shadptr->progText, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_GlyphSlot g = face->glyph;

	for (int i = 0; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}
		wAtlas += g->bitmap.width;
		if (g->bitmap.rows > hAtlas)
			hAtlas = g->bitmap.rows;
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, wAtlas, hAtlas, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int x = 0;
	for (int i = 0; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		characters[i].sizex = g->bitmap.width;
		characters[i].sizey = g->bitmap.rows;
		characters[i].bl = g->bitmap_left;
		characters[i].bt = g->bitmap_top;
		characters[i].ax = g->advance.x;
		characters[i].ay = g->advance.y;
		characters[i].tx = (float)x / wAtlas;

		x += g->bitmap.width;
	}
}

Text * tptr;