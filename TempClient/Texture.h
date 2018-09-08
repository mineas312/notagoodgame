#pragma once
#include "stdafx.h"

#define KEY_COLOR_R 0
#define KEY_COLOR_G 0xFF
#define KEY_COLOR_B 0xFF

class Texture
{
public:
	Texture() noexcept : tex_width{0}, tex_height{0}, width_{0.0f}, height_{0.0f}, range_per_width_tex_{0.0f},
	                     range_per_height_tex_{0.0f}
	{
	}

	void render(const int x, const int y) const;

	void set_texture(const char* path, SDL_Rect* clip = nullptr, int w = 0, int h = 0);

private:
	bool load_texture(const char* path);

	void free() const;

public:
	int tex_width;
	int tex_height;
	GLuint vao{};
	GLuint texture{};
private:
	const uint8_t index_data_[6] = {0, 1, 2, 1, 2, 3};
	GLuint vbo_{};
	GLuint ibo_{};
	float width_;
	float height_;
	float range_per_width_tex_;
	float range_per_height_tex_;
};
