# pragma once

# include <string>
# include <vector>
# include <cstdint>

# include <math.hpp>

struct	Mesh
{
	std::string	  _materialName;
	class Material	* _materialPtr;
	uint32_t	  _indicesStart;
	uint32_t	  _indicesCount;
};

class	Object
{
	public:
		Object();
		~Object();
		
		static Object	* load(const std::string &path);
		void		render(const class Shader &shader);

		const std::string	& getMtlName() const;

		void	setMaterial(const struct MaterialGroup &mats);

	private:
		// buff[0]=>_EBO	| buff[1]=>_VBO 	| buff[2]=>_VAO
		// names[0]=>_name	| names[1]=>_mtlName
		Object(const uint32_t buff[3], const std::string names[2],
				const std::vector<struct Mesh> meshs);
		
		std::vector<struct Mesh>	_meshs;
		
		std::string	_name;
		std::string	_mtlName;
		
		unsigned int	_EBO;
		unsigned int	_VBO;
		unsigned int	_VAO;
};

vec3	readToVec3(const std::vector<std::string> &words);
vec2	readToVec2(const std::vector<std::string> &words);
void	readFace(const std::vector<std::string> &words, const std::vector<vec3> &v, const std::vector<vec2> &vt,
		const std::vector<vec3> &vn, std::vector<float> &vertices, std::vector<uint32_t> &indices, Mesh &mesh);
void	readCorner(const std::string &description, const std::vector<vec3> &v, const std::vector<vec2> &vt,
		const std::vector<vec3> &vn, std::vector<float> &vertices);
