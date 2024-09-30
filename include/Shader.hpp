# pragma once

# include <iostream>

class	Shader
{
	public:
		Shader(const std::string &vertPath, const std::string &fragPath);
		~Shader();

		void	use();
		void	recompile();

	private:
		unsigned int	_id;
		std::string	_vertexPath;
		std::string	_fragmentPath;

		void		_compile();
		std::string	_getShaderSource(const std::string &path);
		unsigned int	_getShaderObject(const char *source, unsigned int type);
};
