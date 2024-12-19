# include <iostream>
# include <string>
# include <fstream>
# include <sstream>

# include <Shader.hpp>
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <math.hpp>

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
	std::cout << "bye bye" << std::endl;
	glUseProgram(0);
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

void	Shader::destroy()
{
	glUseProgram(0);
	glDeleteProgram(this->_id);
}

int	Shader::_getUniformLocation(const std::string &name)
{
	try {
		return (this->_uniformLocations.at(name));
	}
	catch (std::exception &e) {
		std::cout << "Uniform: " << name << " can not be accessed in cach memory.\nRetrivring from shader" << std::endl;

		this->_uniformLocations[name] = glGetUniformLocation(this->_id, name.c_str());
		return (this->_uniformLocations[name]);
	}
}

void	Shader::setBool(const std::string &name, const bool &value)
{
	glUseProgram(this->_id);
	
	int	uniformLocation = _getUniformLocation(name);

	glUniform1i(uniformLocation, (int)value);
}

void	Shader::setInt(const std::string &name, const int &value)
{
	glUseProgram(this->_id);
	
	int	uniformLocation = _getUniformLocation(name);

	glUniform1i(uniformLocation, value);
}

void	Shader::setFloat(const std::string &name, const float &value)
{
	glUseProgram(this->_id);
	
	int	uniformLocation = _getUniformLocation(name);

	glUniform1f(uniformLocation, value);
}

void	Shader::setVec3(const std::string &name, const struct vec3 &value)
{
	glUseProgram(this->_id);
	
	int	uniformLocation = _getUniformLocation(name);

	glUniform3fv(uniformLocation, 1, value.entries);
}

void	Shader::setMat4(const std::string &name, const struct mat4 &value) 
{
	glUseProgram(this->_id);
	
	int	uniformLocation = this->_getUniformLocation(name);

	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value.entries);
}
