#include <SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <cstdio>

int main(int argc, char ** args)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::printf("Failed to init SDL\n");
		return false;
	}
	SDL_Window * window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	SDL_Event e;
	bool quit = false;

	glClearColor(0.0, 1.0, 0.0, 1.0);
	while (!quit)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;
		}

		SDL_GL_SwapWindow(window);
	}
}