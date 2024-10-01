# pragma once

# include <iostream>

class	Shader
{
	public:
		Shader(const std::string &vertPath, const std::string &fragPath);
		~Shader();

		void	use();
		void	recompile();

		void	setBool(const std::string &name, bool value);
		void	setInt(const std::string &name, int value);
		void	setFloat(const std::string &name, float value);
		void	setVec2(const std::string &name, float value1, float value2);
		void	setVec3(const std::string &name, float value1, float value2, float value3);
		void	setVec4(const std::string &name, float value1, float value2, float value3, float value4);

	private:
		unsigned int	_id;
		std::string	_vertexPath;
		std::string	_fragmentPath;

		void		_compile();
		std::string	_getShaderSource(const std::string &path);
		unsigned int	_getShaderObject(const char *source, unsigned int type);
};
