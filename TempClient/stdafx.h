#pragma once
#define _SECURE_SCL 0
#define _ITERATOR_DEBUG_LEVEL 0
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
#include <atomic>
#include <map>

#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

#include <stb_image.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SSE3
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
