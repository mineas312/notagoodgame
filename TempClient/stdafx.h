#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstdio>

#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>

#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SSE3
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
