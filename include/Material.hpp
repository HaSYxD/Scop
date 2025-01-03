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
		Material();
		~Material();
		
		std::string	& getName() const;

		uintptr_t	& getTexture() const;
		
		vec3	& getAmbColor() const;
		vec3	& getDiffColor() const;
		vec3	& getSpecColor() const;
		float	& getSpecExponent() const;

	private:
		std::string	_name;

		uintptr_t	_texture;

		vec3	_ambColor;
		vec3	_diffColor;
		vec3	_SpecColor;
		float	_specExponent;
};

MaterialGroup	loadMtlFile(const std::string &path);
