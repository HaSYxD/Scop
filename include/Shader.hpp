# pragma once

# include <iostream>
# include <map>

class	Shader
{
	public:
		Shader(const std::string &vertPath, const std::string &fragPath);
		~Shader();

		void	use() const;
		void	recompile();
		void	destroy();

		void	setBool(const std::string &name, const bool &value);
		void	setInt(const std::string &name, const int &value);
		void	setFloat(const std::string &name, const float &value);
		void	setVec3(const std::string &name, const struct vec3 &value);
		void	setMat4(const std::string &name, const struct mat4 &value);

	private:
		std::map<const std::string, int>	_uniformLocations;
	
		unsigned int	_id;
		std::string	_vertexPath;
		std::string	_fragmentPath;

		void		_compile();
		std::string	_getShaderSource(const std::string &path);
		unsigned int	_getShaderObject(const char *source, unsigned int type);
		int		_getUniformLocation(const std::string &name);
};
