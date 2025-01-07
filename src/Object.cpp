# include <fstream>
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <cfloat>

# include <Object.hpp>
# include <Shader.hpp>
# include <Material.hpp>
# include <utils.hpp>

Object::Object() : _center(vec3{0, 0, 0}), _EBO(0), _VBO(0), _VAO(0) {}
Object::~Object() {}

Object	* Object::load(const std::string &path)
{
	std::vector<vec3>	v, vn;
	std::vector<vec2>	vt;
	std::vector<float>	vertices;

	size_t	posNum = 0, texNum = 0, normNum = 0, faceNum = 0;

	std::ifstream	file(path);
	std::string	line;

	if (!file.is_open())
		throw(std::runtime_error("could not open .obj file"));

	// counting number of vert/uv/norm... to reserve memory space
	while (std::getline(file, line)) {
		std::vector<std::string>	words = split(line, ' ');
		
		if (words.size() == 0)
			continue ;
		else if (words[0] == "v")
			posNum++;
		else if (words[0] == "vt")
			texNum++;
		else if (words[0] == "vn")
			normNum++;
		else if (words[0] == "f")
			faceNum += words.size() - 3;
	}
	v.reserve(posNum);
	vt.reserve(texNum);
	vn.reserve(normNum);

	std::cout << "Loading:\n - " << posNum << " vertex\n - " << texNum << " tex pos\n - " << normNum << " normals" << std::endl;

	// reserver for each triangles: 3 corner composed of a 3 position floats, 2 texture floats and 3 normal floats
	vertices.reserve(faceNum * 3 * (3 + 2 + 3)); 

	// go back to the start of the file
	file.clear();
	file.seekg(0);

	// actual usage a the obj data
	std::vector<uint32_t>	indices;
	uint32_t		currentMesh = 0;
	Object			*obj = new Object();

	obj->_meshs.push_back(Mesh{"default name", NULL, 0, 0});

	vec3	centerMin = vec3{FLT_MAX, FLT_MAX, FLT_MAX};
	vec3	centerMax = vec3{FLT_MIN, FLT_MIN, FLT_MIN};
	size_t	i = 0;

	while (std::getline(file, line)) {
		std::vector<std::string>	words = split(line, ' ');

		if (words.size() == 0)
			continue ;
		else if (words[0] == "v") {
			v.push_back(readToVec3(words));

			if (v[i].entries[0] < centerMin.entries[0])
				centerMin.entries[0] = v[i].entries[0];
			if (v[i].entries[1] < centerMin.entries[1])
				centerMin.entries[1] = v[i].entries[1];
			if (v[i].entries[2] < centerMin.entries[2])
				centerMin.entries[2] = v[i].entries[2];
			
			if (v[i].entries[0] > centerMax.entries[0])
				centerMax.entries[0] = v[i].entries[0];
			if (v[i].entries[1] > centerMax.entries[1])
				centerMax.entries[1] = v[i].entries[1];
			if (v[i].entries[2] > centerMax.entries[2])
				centerMax.entries[2] = v[i].entries[2];
			i++;
		}
		else if (words[0] == "vt")
			vt.push_back(readToVec2(words));
		else if (words[0] == "vn")
			vn.push_back(readToVec3(words));
		else if (words[0] == "f")
			readFace(words, v, vt, vn, vertices, indices, obj->_meshs[currentMesh]);
		else if (words[0] == "mtllib")
			obj->_mtlName = words[1];
		else if (words[0] == "g") {
			uint32_t	newPos = obj->_meshs[currentMesh]._indicesStart + obj->_meshs[currentMesh]._indicesCount;

			currentMesh++;
			obj->_meshs.push_back(Mesh{"default name", NULL, newPos, 0});
		}
		else if (words[0] == "usemtl") {
			if (obj->_meshs[currentMesh]._materialName == "default name") {
				obj->_meshs[currentMesh]._materialName = words[1];
				continue ;
			}
			uint32_t	newPos = obj->_meshs[currentMesh]._indicesStart + obj->_meshs[currentMesh]._indicesCount;

			currentMesh++;
			obj->_meshs.push_back(Mesh{words[1], NULL, newPos, 0});
		}
	}
	file.close();

	obj->_center = vec3{
		(centerMax.entries[0] + centerMin.entries[0]) / 2,
		(centerMax.entries[1] + centerMin.entries[1]) / 2,
		(centerMax.entries[2] + centerMin.entries[2]) / 2
	};

	std::cout << obj->_center.entries[0] << " " << obj->_center.entries[1] << " " << obj->_center.entries[2] << std::endl;

	std::cout << " - " << obj->_meshs[currentMesh]._indicesCount + obj->_meshs[currentMesh]._indicesStart
		<< " triangles" << std::endl;

	// Object buffer construction
	glGenVertexArrays(1, &obj->_VAO);
	glBindVertexArray(obj->_VAO);

	glGenBuffers(1, &obj->_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, obj->_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &obj->_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);

	return (obj);
}

void	Object::render(class Shader &shader)
{
	shader.use();
	glBindVertexArray(this->_VAO);
	for (Mesh &mesh : this->_meshs) {
		shader.setVec3("ambColor", mesh._materialPtr->getAmbColor());
		shader.setVec3("diffColor", mesh._materialPtr->getDiffColor());
		shader.setVec3("specColor", mesh._materialPtr->getSpecColor());
		shader.setFloat("specExponent", mesh._materialPtr->getSpecExponent());

		glDrawElements(GL_TRIANGLES, mesh._indicesCount, GL_UNSIGNED_INT, (void *)(mesh._indicesStart * sizeof(unsigned int)));
	}
	glBindVertexArray(0);
}

void	Object::setMaterial(struct MaterialGroup *mats)
{
	for (Mesh &msh : this->_meshs)
		for (Material &mat : mats->_materials)
			if (msh._materialName == mat.getName())
				msh._materialPtr = &mat;
}

const std::string	& Object::getMtlName() const
{
	return (this->_mtlName);
}

void	readFace(const std::vector<std::string> &words, const std::vector<vec3> &v, const std::vector<vec2> &vt,
		const std::vector<vec3> &vn, std::vector<float> &vertices, std::vector<uint32_t> &indices, Mesh &mesh)
{
	size_t	triangleCount = words.size() - 3;

	for (size_t i = 0; i < triangleCount; i++) {
		readCorner(words[1], v, vt, vn, vertices);
		readCorner(words[2 + i], v, vt, vn, vertices);
		readCorner(words[3 + i], v, vt, vn, vertices);

		indices.push_back(mesh._indicesStart + mesh._indicesCount);
		indices.push_back(mesh._indicesStart + mesh._indicesCount + 1);
		indices.push_back(mesh._indicesStart + mesh._indicesCount + 2);
		mesh._indicesCount += 3;
	}
}

void	readCorner(const std::string &description, const std::vector<vec3> &v, const std::vector<vec2> &vt,
		const std::vector<vec3> &vn, std::vector<float> &vertices)
{
	std::vector<std::string>	spDesc = split(description, '/');

	// Positions (vec3)
	if (spDesc.size() == 0 || spDesc[0].empty()) {
		vertices.push_back(0);
		vertices.push_back(0);
		vertices.push_back(0);
	}
	else {
		size_t	vIndex = atol(spDesc[0].c_str()) - 1;

		vertices.push_back(v[vIndex].entries[0]);
		vertices.push_back(v[vIndex].entries[1]);
		vertices.push_back(v[vIndex].entries[2]);
	}

	// Texture coordinates (vec2)
	if (spDesc.size() <= 1 || spDesc[1].empty()) {
		vertices.push_back(0);
		vertices.push_back(0);
	}
	else {
		size_t	vtIndex = atol(spDesc[1].c_str()) - 1;

		vertices.push_back(vt[vtIndex].entries[0]);
		vertices.push_back(vt[vtIndex].entries[1]);
	}
	
	// Normals (vec3)
	if (spDesc.size() <= 2 || spDesc[2].empty()) {
		vertices.push_back(1);
		vertices.push_back(1);
		vertices.push_back(1);
	}
	else {
		size_t	vnIndex = atol(spDesc[2].c_str()) - 1;

		vertices.push_back(vn[vnIndex].entries[0]);
		vertices.push_back(vn[vnIndex].entries[1]);
		vertices.push_back(vn[vnIndex].entries[2]);
	}
}

const vec3	& Object::getCenter() const
{
	return (this->_center);
}
