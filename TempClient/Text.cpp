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
	FT_Set_Pixel_Sizes(face, 0, TEXT_HEIGHT);

	createTextureAtlas();
}

void Text::render(const std::string& text, GLfloat x, GLfloat y, const GLfloat scale, const glm::vec3& color)
{
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, tex);

	// Hardcoded location 3 in text.frag shader
	glUniform3f(3, color.x, color.y, color.z);

	alignas(16) GLfloat(*verts)[6][4] = new GLfloat[text.size()][6][4];

	const __m128 divisors = _mm_set_ps(wAtlas, hAtlas, 1.0f, 1.0f);
	const __m128 fontScale = _mm_set1_ps(scale);
	
	int n = 0;
	for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		const Charact ch = characters[*c];

		__m128 fData = _mm_loadu_ps((const float*)&ch);
		fData = _mm_shuffle_ps(fData, fData, _MM_SHUFFLE(1, 0, 1, 2));
		
		alignas(16) float fDataArr[4];
		_mm_store_ps(fDataArr, _mm_mul_ps(_mm_sub_ps(fData, _mm_set_ps(0.0f, 0.0f, ch.bt, 0.0f)), fontScale));

		const GLfloat xpos = x + fDataArr[0];
		const GLfloat ypos = y - fDataArr[1];
		const GLfloat w = fDataArr[2];
		const GLfloat h = fDataArr[3];

		const __m128 additions = _mm_set_ps(ch.tx, 0.0f, w, h);
		__m128 results = _mm_set_ps(ch.sizex, ch.sizey, xpos, ypos);

		results = _mm_div_ps(results, divisors);

		results = _mm_add_ps(results, additions);

		alignas(16) float resArr[4];
		_mm_store_ps(resArr, results);
		
		resArr[2] = 1.0f - (1.0f - resArr[2]);

		GLfloat vertices[6][4]{
			{xpos, resArr[0], ch.tx, 0.0f},
			{xpos, ypos, ch.tx,	resArr[2]},
			{resArr[1], ypos, resArr[3], resArr[2]},

			{xpos, resArr[0], ch.tx, 0.0f},
			{resArr[1], ypos, resArr[3], resArr[2]},
			{resArr[1], resArr[0], resArr[3], 0.0f}
		};


		std::memcpy(verts[n], vertices, 6 * 4 * sizeof(float));

		x += ((GLuint)ch.ax >> 6) * scale;
		y += ((GLuint)ch.ay >> 6) * scale;
		n++;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4 * n, verts, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, n * 6);

	delete[] verts;
}

void Text::renderPlainText(const GLfloat scale)
{
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, tex);

	glUniform3f(3, 255, 255, 255);

	int size = 0;
	for (PlainText & p : toRender)
		size += p.text.size();

	alignas(16) GLfloat(*verts)[6][4] = new GLfloat[size][6][4];

	const __m128 divisors = _mm_set_ps(wAtlas, hAtlas, 1.0f, 1.0f);
	const __m128 fontScale = _mm_set1_ps(scale);

	int n = 0;
	for (PlainText & p : toRender)
	{
		for (std::string::const_iterator c = p.text.begin(); c != p.text.end(); c++)
		{
			const Charact ch = characters[*c];

			__m128 fData = _mm_loadu_ps((const float*)&ch);
			fData = _mm_shuffle_ps(fData, fData, _MM_SHUFFLE(1, 0, 1, 2));

			alignas(16) float fDataArr[4];
			_mm_store_ps(fDataArr, _mm_mul_ps(_mm_sub_ps(fData, _mm_set_ps(0.0f, 0.0f, ch.bt, 0.0f)), fontScale));

			const GLfloat xpos = p.x + fDataArr[0];
			const GLfloat ypos = p.y - fDataArr[1];
			const GLfloat w = fDataArr[2];
			const GLfloat h = fDataArr[3];

			const __m128 additions = _mm_set_ps(ch.tx, 0.0f, w, h);
			__m128 results = _mm_set_ps(ch.sizex, ch.sizey, xpos, ypos);

			results = _mm_div_ps(results, divisors);

			results = _mm_add_ps(results, additions);

			alignas(16) float resArr[4];
			_mm_store_ps(resArr, results);

			resArr[2] = 1.0f - (1.0f - resArr[2]);

			GLfloat vertices[6][4]{
				{ xpos, resArr[0], ch.tx, 0.0f },
			{ xpos, ypos, ch.tx,	resArr[2] },
			{ resArr[1], ypos, resArr[3], resArr[2] },

			{ xpos, resArr[0], ch.tx, 0.0f },
			{ resArr[1], ypos, resArr[3], resArr[2] },
			{ resArr[1], resArr[0], resArr[3], 0.0f }
			};


			std::memcpy(verts[n], vertices, 6 * 4 * sizeof(float));

			p.x += ((GLuint)ch.ax >> 6) * scale;
			p.y += ((GLuint)ch.ay >> 6) * scale;
			n++;
		}
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4 * n, verts, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, n * 6);

	delete[] verts;
	toRender.clear();
}

void Text::addTextToRender(const std::string & text, GLfloat x, GLfloat y)
{
	PlainText p;
	p.text = text;
	p.x = x;
	p.y = y;
	toRender.push_back(p);
}

void Text::createTextureAtlas()
{
	glUseProgram(shadptr->progText);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	
	projection = glm::ortho(0.0f, (float)winptr->SCREEN_WIDTH, 0.0f, (float)winptr->SCREEN_HEIGHT);
	glUniformMatrix4fv(glGetUniformLocation(shadptr->progText, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_GlyphSlot g = face->glyph;

	for (int i = 32; i < 128; i++) {
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
	
	glActiveTexture(GL_TEXTURE0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, wAtlas, hAtlas, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int x = 0;
	for (int i = 32; i < 128; i++) {
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