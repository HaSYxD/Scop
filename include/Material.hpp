# pragma once

# include <cstdint>
# include <string>
# include <vector>

# include <math.hpp>

typedef struct	MaterialGroup
{
	std::string			_mtlFileName;
	std::vector<class Material>	_materials;
}	MaterialGroup;

class	Material
{
	public:
		~Material();

		static MaterialGroup	* loadMtlFile(const std::string &path);
		
		std::string	& getName();
		uint32_t	& getTexture();
		
		vec3	& getAmbColor();
		vec3	& getDiffColor();
		vec3	& getSpecColor();
		float	& getSpecExponent();

	private:
		Material(const std::string &name);

		std::string	_name;

		vec3	_ambColor;
		vec3	_diffColor;
		vec3	_specColor;
		float	_specExponent;
		uint32_t	_texture;
};

