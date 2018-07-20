#include "stdafx.h"
#include "Shader.h"

Shader::Shader() noexcept : isValid{ false }, ProgID{ 0 }, VSID{ 0 }, FSID{ 0 }
{}

Shader::Shader(const char * vs, const char * fs) : isValid{ false }, ProgID{ 0 }, VSID{ 0 }, FSID{ 0 }
{
	isValid = InitShader(vs, fs);
}

bool Shader::InitShader(const char * vs, const char * fs)
{
	std::string tmp;
	
	// Vertex shader
	std::stringstream vsSS;
	std::fstream vsFile(vs, std::ios::in | std::ios::beg);

	while (std::getline(vsFile, tmp))
	{
		vsSS << tmp << "\n";
	}
	
	std::string vsCode = vsSS.str();
	GLchar* vsPomocy = new GLchar[vsCode.size() + 1];
	std::memcpy(vsPomocy, vsCode.data(), vsCode.size());
	vsPomocy[vsCode.size()] = '\0';

	VSID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VSID, 1, &vsPomocy, NULL);
	glCompileShader(VSID);
	
	// Fragment shader
	std::stringstream fsSS;
	std::fstream fsFile(fs, std::ios::in | std::ios::beg);
	while (std::getline(fsFile, tmp))
	{
		fsSS << tmp << "\n";
	}

	std::string fsCode = fsSS.str();
	GLchar* fsPomocy = new GLchar[fsCode.size()+1];
	std::memcpy(fsPomocy, fsCode.data(), fsCode.size());
	fsPomocy[fsCode.size()] = '\0';

	FSID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FSID, 1, &fsPomocy, NULL);
	glCompileShader(FSID);

	// Cleanup
	vsFile.close();
	fsFile.close();
	delete[] vsPomocy;
	delete[] fsPomocy;

	Log(VSID);
	Log(FSID);

	ProgID = glCreateProgram();

	glAttachShader(ProgID, VSID);
	glAttachShader(ProgID, FSID);
	glLinkProgram(ProgID);

	//// Potencjalnie wielow¹tkowe renderowanie shaderów
	//std::string tmp;
	//
	//// Vertex Shader
	//std::ifstream vsFile(vs);
	//if (!vsFile.good())
	//{
	//	fprintf(stderr, "Error opening Vertex Shader file");
	//	vsFile.close();
	//	isValid = false;
	//	return false;
	//}

	//GLchar * vsSource;

	//int length;
	//vsFile.seekg(0, std::ios::end);
	//length = vsFile.tellg();
	//vsFile.seekg(0, std::ios::beg);
	//vsSource = new GLchar[length+1];
	//vsFile.read(vsSource, length);

	//VSID = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(VSID, 1, &vsSource, NULL);
	//glCompileShader(VSID);
	//vsFile.close();

	//std::ifstream fsFile(fs);
	//if (!fsFile.good())
	//{
	//	fprintf(stderr, "Error opening Fragment Shader file");
	//	fsFile.close();
	//	isValid = false;
	//	return false;
	//}

	//GLchar * fsSource;

	//int length2;
	//fsFile.seekg(0, std::ios::end);
	//length2 = fsFile.tellg();
	//fsFile.seekg(0, std::ios::beg);
	//fsSource = new GLchar[length2+1];
	//fsFile.read(fsSource, length2);

	//FSID = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(FSID, 1, &fsSource, NULL);
	//glCompileShader(FSID);

	//Log(VSID);
	//Log(FSID);

	//ProgID = glCreateProgram();

	//glAttachShader(ProgID, VSID);
	//glAttachShader(ProgID, FSID);
	//glLinkProgram(ProgID);

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