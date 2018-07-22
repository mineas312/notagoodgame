#include "stdafx.h"
#include "Camera.h"


Camera::Camera() : proj { glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f) }
{
}


Camera::~Camera()
{
}

void Camera::init()
{
	cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraTarget, cameraDirection);
	GLint cw[4];
	glGetIntegerv(GL_VIEWPORT, cw);
	camRect.x = static_cast<int>(cameraPos.x / winptr->rangePerWidthPixel) + (winptr->VIEW_WIDTH);
	camRect.y = static_cast<int>(-cameraPos.y / winptr->rangePerHeightPixel) + (winptr->VIEW_HEIGHT);
	camRect.w = cw[2] / 2;
	camRect.h = cw[3] / 2;
}

void Camera::center(const int x, const int y, Map &m) noexcept
{
	cameraPos.x = x * (winptr->rangePerWidthPixel);
	cameraPos.y = -y * (winptr->rangePerHeightPixel);
	cameraTarget.x = x * (winptr->rangePerWidthPixel);
	cameraTarget.y = -y * (winptr->rangePerHeightPixel);
	view = glm::lookAt(cameraPos, cameraTarget, cameraDirection);

	camRect.x = static_cast<int>((cameraPos.x / winptr->rangePerWidthPixel) + (winptr->VIEW_WIDTH));
	camRect.y = static_cast<int>((-cameraPos.y / winptr->rangePerHeightPixel) + (winptr->VIEW_HEIGHT));

	if (cameraPos.x < 0.0f)
		cameraPos.x = 0.0f;
	if (cameraPos.y < 0.0f)
		cameraPos.y = 0.0f;
}

Camera * camptr;