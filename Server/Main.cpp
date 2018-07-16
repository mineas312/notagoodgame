#include <SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <cstdio>
#define GLM_FORCE_SSE3
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SDL_GLContext context;
SDL_Window * window;
GLuint program;
SDL_Event e;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 720;

float rangePerWidthPixel = 2.0f / WINDOW_WIDTH;
float rangePerHeightPixel = 2.0f / WINDOW_HEIGHT;

void printProgramLog(GLuint program)
{
	if (glIsProgram(program))
	{
		int infoLogLength = 0, maxLength = 0;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		char * infoLog = new char[maxLength];

		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
			printf("PROGRAM ERROR:\n%s\n", infoLog);

		delete[] infoLog;
	}
	else
		printf("Name %d is not a program\n", program);
}

void printShaderLog(GLuint shader)
{
	if (glIsShader(shader))
	{
		int infoLogLength = 0, maxLenght = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLenght);

		char * infoLog = new char[maxLenght];

		glGetShaderInfoLog(shader, maxLenght, &infoLogLength, infoLog);
		if (infoLogLength > 0)
			printf("PROGRAM ERROR:\n%s\n", infoLog);

		delete[] infoLog;
	}
	else
		printf("Name %d is not a shader", shader);
}

void init()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
		std::printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));

	program = glCreateProgram();
}

void compileShaders()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar * vertexShaderSource[] = { "#version 450\nlayout(location = 0) in vec3 LVertexPos2D; uniform mat4 model; void main() { gl_Position = model*vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }" };

	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d\n", vertexShader);
		printShaderLog(vertexShader);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar * fragmentShaderSource[] = { "#version 450\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }" };

	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	GLint fShaderCompile = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompile);
	if (fShaderCompile != GL_TRUE)
	{
		printf("Unable to compile fragment shader %d\n", fragmentShader);
		printShaderLog(fragmentShader);
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint programSuccess = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Unable to link program %d", program);
		printProgramLog(program);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

class Object
{
public:
	Object()
	{
		width = 0.0f;
		height = 0.0f;
	}
	~Object()
	{
	}
	void render(int x, int y)
	{
		glBindVertexArray(vao);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * rangePerWidthPixel, -y * rangePerHeightPixel, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	}
	void setObject(int w, int h)
	{
		width = w * rangePerWidthPixel;
		height = h * rangePerHeightPixel;

		vertices[0] = { -1.0f, 1.0f, 0.0f };
		vertices[1] = { -1.0f + width, 1.0f, 0.0f };
		vertices[2] = { -1.0f, 1.0f - height, 0.0f };
		vertices[3] = { -1.0f + width, 1.0f - height, 0.0f };

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
	}
public:
	GLuint indexData[6] = { 0, 1, 2, 1, 2, 3 };
	glm::vec3 vertices[4];
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	float width;
	float height;
};

int main(int argc, char ** args)
{
	init();
	compileShaders();

	bool quit = false;

	Object rect;
	rect.setObject(100, 100);

	glClearColor(0.0, 0.0, 0.5, 1.0);

	int x = 0, y = 0;

	glUseProgram(program);

	while (!quit)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.key.keysym.sym == SDLK_w)
			{
				y -= 3;
			}
			if (e.key.keysym.sym == SDLK_d)
			{
				x += 3;
			}
			if (e.key.keysym.sym == SDLK_s)
			{
				y += 3;
			}
			if (e.key.keysym.sym == SDLK_a)
			{
				x -= 3;
			}
		}

		rect.render(x, y);

		SDL_GL_SwapWindow(window);
	}

	return 0;
}