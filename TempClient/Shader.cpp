#include "stdafx.h"
#include "Shader.h"
#include "ThreadPool.h"

void Shader::initShader(const char* vs, const char* fs, GLuint &program)
{
	std::string tmp;

	GLchar* vsCodeCstr;
	GLchar* fsCodeCstr;
	
	// Vertex shader
	std::stringstream vsSS;
	std::fstream vsFile(vs, std::ios::in | std::ios::beg);

	while (std::getline(vsFile, tmp)) {
		vsSS << tmp << "\n";
	}

	std::string vsCode = vsSS.str();
	vsCodeCstr = new GLchar[vsCode.size() + 1];
	std::memcpy(vsCodeCstr, vsCode.data(), vsCode.size());
	vsCodeCstr[vsCode.size()] = '\0';

	vsFile.close();

	// Fragment shader
	std::stringstream fsSS;
	std::fstream fsFile(fs, std::ios::in | std::ios::beg);
	while (std::getline(fsFile, tmp)) {
		fsSS << tmp << "\n";
	}

	std::string fsCode = fsSS.str();
	fsCodeCstr = new GLchar[fsCode.size() + 1];
	std::memcpy(fsCodeCstr, fsCode.data(), fsCode.size());
	fsCodeCstr[fsCode.size()] = '\0';

	fsFile.close();

	// Compilation
	GLuint VSID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VSID, 1, &vsCodeCstr, NULL);
	glCompileShader(VSID);
	
	GLuint FSID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FSID, 1, &fsCodeCstr, NULL);
	glCompileShader(FSID);

	// Cleanup
	delete[] vsCodeCstr;
	delete[] fsCodeCstr;

	Log(VSID);
	Log(FSID);

	program = glCreateProgram();

	glAttachShader(program, VSID);
	glAttachShader(program, FSID);
	glLinkProgram(program);

	Log(program);
}

void Shader::init()
{
	shadptr->initShader("res/shaders/tiles.vert", "res/shaders/tiles.frag", progGraphics);
	shadptr->initShader("res/shaders/text.vert", "res/shaders/text.frag", progText);
}

void Shader::Log(GLuint ID)
{
	GLsizei bufSize = 0;
	GLsizei logLength = 0;

	if (glIsShader(ID))
	{
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &bufSize);
		auto log = std::vector<GLchar>(static_cast<uint64_t>(bufSize));

		glGetShaderInfoLog(ID, bufSize, &logLength, log.data());
		if (logLength != 0)
		{
			std::fprintf(stderr, "Shader program log : %s\n", log.data());
			std::printf("Shader program log: %s\n", log.data());
		}
	}
	else if (glIsProgram(ID))
	{
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &bufSize);
		auto log = std::vector<GLchar>(static_cast<uint64_t>(bufSize));

		glGetProgramInfoLog(ID, bufSize, &logLength, log.data());
		if (logLength != 0)
		{
			std::fprintf(stderr, "Shader program log : %s\n", log.data());
			std::printf("Shader program log: %s\n", log.data());
		}
	}
}
Shader * shadptr;