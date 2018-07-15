#include "stdafx.h"

#include "Window.h"

// -Initializes window, renderer

void Window::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Cannot init SDL. Error: %s\n", SDL_GetError());
		exit(-1);
	}

	window = SDL_CreateWindow("Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		fprintf(stderr, "Cannot create window. Error: %s\n", SDL_GetError());
		exit(-1);
	}

	setOpenGLAtrributes();
	context = SDL_GL_CreateContext(window);
	if (context == NULL)
	{
		fprintf(stderr, "Cannot create renderer. Error: %s\n", SDL_GetError());
		exit(-1);
	}
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		fprintf(stderr, "Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}
	// Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		fprintf(stderr, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	if (!initGL())
	{
		printf("Unable to initialize OpenGL!\n");
		fprintf(stderr, "Unable to initialize OpenGL!\n");
		exit(-1);
	}
	//Init PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		fprintf(stderr, "Cannot init PNG loading. Error: %s\n", IMG_GetError());
		exit(-1);
	}
	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

void Window::centerCamera(const int x, const int y, Map & map)
{
	camera.x = x - SCREEN_WIDTH / 2;
	camera.y = y - SCREEN_HEIGHT / 2;

	if (camera.x < 0)
		camera.x = 0;

	if (camera.x > map.width - camera.w)
		camera.x = map.width - camera.w;

	if (camera.y < 0)
		camera.y = 0;

	if (camera.y > map.height - camera.h)
		camera.y = map.height - camera.h;
}

void Window::close()
{
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_GL_DeleteContext(context);
	context = NULL;
}

void Window::setOpenGLAtrributes()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void Window::printProgramLog(GLuint program)
{
	if (glIsProgram(program))
	{
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		char * infoLog = new char[maxLength];

		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
			printf("%s\n", infoLog);
		delete[] infoLog;
	}
	else
		printf("Name %d is not a program\n", program);
}

void Window::printShaderLog(GLuint shader)
{
	if (glIsProgram(shader))
	{
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		char * infoLog = new char[maxLength];

		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
			printf("%s\n", infoLog);
		delete[] infoLog;
	}
	else
		printf("Name %d is not a shader\n", shader);
}

bool Window::initGL()
{
	bool success = true;

	programID = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar * vertexShaderSource[] =
	{
		"#version 450\nlayout(location = 0) in vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

	glCompileShader(vertexShader);

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		printShaderLog(vertexShader);
		success = false;
	}
	else
	{
		glAttachShader(programID, vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar * fragmentShaderSource[] =
		{
			"#version 450\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
		};

		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		glCompileShader(fragmentShader);

		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile fragment shader %d!\n", fragmentShader);
			printShaderLog(fragmentShader);
			success = false;
		}
		else
		{
			glAttachShader(programID, fragmentShader);

			glLinkProgram(programID);

			GLint programSuccess = GL_TRUE;
			glGetProgramiv(programID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", programID);
				printProgramLog(programID);
				success = false;
			}
			else
			{
				vertexPos2DLocation = glGetAttribLocation(programID, "LVertexPos2D");
				if (vertexPos2DLocation == -1)
				{
					printf("LVertexPos2D is not a valid glsl program variable!\n");
					success = false;
				}
				// TODO DELETE BECAUSE DEBUG
				else
				{
					glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

					//VBO data 
					GLfloat vertexData[] = { 
						-0.5f, -0.5f,
						 0.5f, -0.5f,
						 0.5f,  0.5f, 
						-0.5f,  0.5f
					};

					//IBO data 
					GLuint indexData[] = { 0, 1, 2, 0, 2, 3 };

					glGenVertexArrays(1, &vao);
					glBindVertexArray(vao);

					//Create VBO
					glGenBuffers(1, &vbo);
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
					glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
					glEnableVertexAttribArray(0);

					//Create IBO
					glGenBuffers(1, &ibo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

				}
			}
		}
	}
	return success;
}

Window* winptr;
