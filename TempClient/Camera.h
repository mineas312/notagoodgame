#pragma once
#include "stdafx.h"
#include "Window.h"

class Camera
{
public:
	Camera();
	~Camera();
	
	void init();
	
	void center(int x, int y, Map &m) noexcept;

public:
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	glm::mat4 view;
	glm::mat4 proj;
	SDL_Rect camRect;
};

extern Camera * camptr;