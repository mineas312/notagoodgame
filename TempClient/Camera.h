#pragma once
#include "stdafx.h"
#include "Window.h"

class Camera
{
public:
	Camera();
	~Camera();
	
	void init()
	{
		cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
		cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		proj = glm::ortho(-1.0f, 0.2f, -0.2f, 1.0f);
		view = glm::lookAt(cameraPos, cameraTarget, cameraDirection);
		GLint cw[4];
		glGetIntegerv(GL_VIEWPORT, cw);
		camRect.x = cameraPos.x / winptr->rangePerWidthPixel;
		camRect.y = cameraPos.y / winptr->rangePerHeightPixel;
		camRect.w = cw[2] * 0.2f;
		camRect.h = cw[3] * 0.2f;
	}

	void center(int x, int y)
	{
		cameraPos.x = x * winptr->rangePerWidthPixel;
		cameraPos.y = -y * winptr->rangePerHeightPixel;
		cameraTarget.x = x * winptr->rangePerWidthPixel;
		cameraTarget.y = y * winptr->rangePerHeightPixel;
		view = glm::lookAt(cameraPos, cameraTarget, cameraDirection);
		camRect.x = cameraPos.x / winptr->rangePerWidthPixel;
		camRect.y = cameraPos.y / winptr->rangePerHeightPixel;
	}

public:
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	glm::mat4 view;
	glm::mat4 proj;
	SDL_Rect camRect;
};

extern Camera * camptr;