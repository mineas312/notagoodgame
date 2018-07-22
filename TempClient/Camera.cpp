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

	camRect.x = static_cast<int>((cameraPos.x / winptr->rangePerWidthPixel) + (winptr->VIEW_WIDTH));
	camRect.y = static_cast<int>((-cameraPos.y / winptr->rangePerHeightPixel) + (winptr->VIEW_HEIGHT));

	if (cameraPos.x < 0.0f - winptr->VIEW_WIDTH * winptr->rangePerWidthPixel)
	{
		cameraPos.x = 0.0f - winptr->VIEW_WIDTH * winptr->rangePerWidthPixel;
		cameraTarget.x = 0.0f - winptr->VIEW_WIDTH * winptr->rangePerWidthPixel;
		camRect.x = 0;
	}
	else if (cameraPos.x / winptr->rangePerWidthPixel > m.width - winptr->VIEW_WIDTH*3)
	{
		cameraPos.x = (m.width - winptr->VIEW_WIDTH * 3) * winptr->rangePerWidthPixel;
		cameraTarget.x = (m.width - winptr->VIEW_WIDTH * 3) * winptr->rangePerWidthPixel;
		camRect.x = m.width - winptr->VIEW_WIDTH * 2;
	}
	if (cameraPos.y > 1.0f - winptr->VIEW_HEIGHT * winptr->rangePerHeightPixel)
	{
		cameraPos.y = 1.0f - winptr->VIEW_HEIGHT * winptr->rangePerHeightPixel;
		cameraTarget.y = 1.0f - winptr->VIEW_HEIGHT * winptr->rangePerHeightPixel;
		camRect.y = 0;
	}
	else if (-cameraPos.y / winptr->rangePerHeightPixel > m.height - winptr->VIEW_HEIGHT * 3)
	{
		cameraPos.y = -(m.height - winptr->VIEW_HEIGHT * 3) * winptr->rangePerHeightPixel;
		cameraTarget.y = -(m.height - winptr->VIEW_HEIGHT * 3) * winptr->rangePerHeightPixel;
		camRect.y = m.height - winptr->VIEW_HEIGHT * 2;
	}

	view = glm::lookAt(cameraPos, cameraTarget, cameraDirection);
}

Camera * camptr;