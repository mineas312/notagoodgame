#pragma once
#include <SDL.h>
#include <type_traits>
#include <limits>

#include "Game.h"
#include "Camera.h"
#include "Window.h"

static const glm::mat4 identityMatrix = glm::mat4(1.0f);

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) noexcept;

template<class T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
bool isEqf(const T a, const T b, const uint8_t ulp) noexcept
{
	return std::abs(a - b) <= std::numeric_limits<T>::epsilon() * std::abs(a + b) * ulp || std::abs(a - b) < std::numeric_limits<T>::min();
}

template<class T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
glm::mat4 calcMVP(const T x, const T y) noexcept
{
	const glm::mat4 model = glm::translate(identityMatrix, glm::vec3(x * winptr->rangePerWidthPixel, -y * winptr->rangePerHeightPixel, 0.0f));

	glm::mat4 mvp = camptr->proj * camptr->view * model;

	return mvp;
}