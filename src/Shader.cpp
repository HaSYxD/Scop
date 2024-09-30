#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <Shader.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::string	Shader::_getShaderSource(const std::string &path)
{
	std::ifstream		fileStream;
	std::stringstream	buffStream;

	fileStream.open(path.c_str());
	buffStream << fileStream.rdbuf();
	fileStream.close();

	std::string	source = buffStream.str();

	return (source);
}

unsigned int	Shader::_getShaderObject(const char *source, unsigned int type)
{
	unsigned int	shader = glCreateShader(type);	//Create Shader object
	
	glShaderSource(shader, 1, &source, NULL);	//Bind Shader source to shader object
	glCompileShader(shader);

	int	compilationStatus = 0;
	char	compilationLog[512] = {0};

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationStatus);
	if (!compilationStatus) {
		glGetShaderInfoLog(shader, 512, NULL, compilationLog);
		std::string	error =  "ERROR: " + std::to_string(type) + " compilation failed\n" + compilationLog;
		throw (std::runtime_error(error));
	}
	return (shader);
}

void	Shader::_compile()
{
	std::string	vertSrc = this->_getShaderSource(this->_vertexPath);
	std::string	fragSrc = this->_getShaderSource(this->_fragmentPath);

	unsigned int	vertexShader;
	unsigned int	fragmentShader;

	try
	{
		vertexShader = this->_getShaderObject(vertSrc.c_str(), GL_VERTEX_SHADER);
		fragmentShader = this->_getShaderObject(fragSrc.c_str(), GL_FRAGMENT_SHADER);

	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	this->_id = glCreateProgram();
	glAttachShader(this->_id, vertexShader);
	glAttachShader(this->_id, fragmentShader);
	glLinkProgram(this->_id);

	int	linkStatus = 0;
	char	linkInfo[512] = {0};

	glGetProgramiv(this->_id, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		glGetProgramInfoLog(this->_id, 512, NULL, linkInfo);
		std::string	error = "ERROR: Shader Program creation failed\n" + std::string(linkInfo);
		std::cerr << error << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader(const std::string &vertPath, const std::string &fragPath)
{
	this->_vertexPath = vertPath;
	this->_fragmentPath = fragPath;
	this->_compile();
}

Shader::~Shader()
{
	glDeleteProgram(this->_id);
}

void	Shader::recompile()
{
	glUseProgram(0);
	glDeleteProgram(this->_id);
	this->_compile();	
}

void	Shader::use()
{
	glUseProgram(this->_id);
}
