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
		~Object();
		
		static Object	* load(const std::string &path);
		void		render(class Shader &shader);

		const std::string	& getMtlName() const;

		void	setMaterial(struct MaterialGroup *mats);

	private:
		Object();
		
		std::vector<struct Mesh>	_meshs;
		
		std::string	_mtlName;
		
		unsigned int	_EBO;
		unsigned int	_VBO;
		unsigned int	_VAO;
};

void	readFace(const std::vector<std::string> &words, const std::vector<vec3> &v, const std::vector<vec2> &vt,
		const std::vector<vec3> &vn, std::vector<float> &vertices, std::vector<uint32_t> &indices, Mesh &mesh);
void	readCorner(const std::string &description, const std::vector<vec3> &v, const std::vector<vec2> &vt,
		const std::vector<vec3> &vn, std::vector<float> &vertices);
