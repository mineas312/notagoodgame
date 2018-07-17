#include "stdafx.h"
#include "Shader.h"

Shader::Shader() noexcept : isValid{ false }, VSID{ 0 }, FSID{ 0 }, ProgID{ 0 }
{}

Shader::Shader(const char * vs, const char * fs) : isValid{ false }, VSID { 0 }, FSID{ 0 }, ProgID{ 0 }
{
	isValid = InitShader(vs, fs);
}

bool Shader::InitShader(const char * vs, const char * fs)
{
	// Potencjalnie wielow¹tkowe renderowanie shaderów
	std::string tmp;
	
	// Vertex Shader
	std::ifstream vsFile(vs);
	if (!vsFile.good())
	{
		fprintf(stderr, "Error opening Vertex Shader file");
		vsFile.close();
		isValid = false;
		return false;
	}

	while (std::getline(vsFile, tmp))
		vsCode << tmp;

	const GLchar* vsSource = vsCode.str().c_str();
	const GLint vsSize = static_cast<GLint>(std::strlen(vsSource));

	VSID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VSID, 1, &vsSource, &vsSize);
	glCompileShader(VSID);
	vsFile.close();

	std::ifstream fsFile(fs);
	if (!fsFile.good())
	{
		fprintf(stderr, "Error opening Fragment Shader file");
		fsFile.close();
		isValid = false;
		return false;
	}
	while (std::getline(fsFile, tmp))
		fsCode << tmp;
	fsFile.close();

	const GLchar* fsSource = fsCode.str().c_str();
	const GLint fsSize = static_cast<GLint>(std::strlen(fsSource));

	FSID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FSID, 1, &fsSource, &fsSize);
	glCompileShader(FSID);

	Log(VSID);
	Log(FSID);

	ProgID = glCreateProgram();

	glAttachShader(ProgID, VSID);
	glAttachShader(ProgID, FSID);
	glLinkProgram(ProgID);

	Log(ProgID);

	isValid = true;
	return true;
}

void Shader::Log(GLuint ID)
{
	GLsizei bufSize = 0;
	GLsizei logLength = 0;

	if (glIsShader(ID))
	{
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &bufSize);
		auto log = std::vector<GLchar>(bufSize);

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
		auto log = std::vector<GLchar>(bufSize);

		glGetProgramInfoLog(ID, bufSize, &logLength, log.data());
		if (logLength != 0)
		{
			std::fprintf(stderr, "Shader program log : %s\n", log.data());
			std::printf("Shader program log: %s\n", log.data());
		}
	}
}
