# include <fstream>
# include <glad/glad.h>
# include <GLFW/glfw3.h>

# include <Object.hpp>
# include <Shader.hpp>
# include <Material.hpp>
# include <utils.hpp>

Object::Object()
{
	//Creating vertex Buffer
	float	vertices[] = {
		 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned int	indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	glGenBuffers(1, &this->_VBO);		//Generating a vertex buffer object on the GPU and getting it's ID
	
	glGenBuffers(1, &this->_EBO);

	glGenVertexArrays(1, &this->_VAO);	//Generating a vertex array object on the GPU and getting it's ID

	glBindVertexArray(this->_VAO);						//Bind Vertex Array object to store vertex attribute calls
	
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);				//Binding the vertex buffer object to the target "GL_ARRAY_BUFFER"
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//Send "vertices" into "GL_ARRAY_BUFFER" wich is bound to "vertexBufferObject"

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);			//Binding "elementBufferObject" to "GL_ELEMENT_ARRAY_BUFFER"
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//Send "indices" into "GL_ELEMENT_ARRAY_BUFFER"
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);


}

Object::Object(const uint32_t buff[3], const std::string names[2],
		const std::vector<struct Mesh> meshs) : _name(names[0]), _mtlName(names[1]), _EBO(buff[0]),
									_VBO(buff[1]), _VAO(buff[2])
{
	(void)meshs;
}

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
	std::vector<uint32_t>		indices;
	std::vector<struct Mesh>	meshs;
	std::string			names[2];
	uint32_t			currentMesh = 0;

	meshs[currentMesh] = (Mesh){"default name", NULL, 0, 0};

	while (std::getline(file, line)) {
		std::vector<std::string>	words = split(line, ' ');

		if (words.size() == 0) {
			continue ;
		}
		else if (words[0] == "v") {
			v.push_back(readToVec3(words));
		}
		else if (words[0] == "vt") {
			vt.push_back(readToVec2(words));
		}
		else if (words[0] == "vn") {
			vn.push_back(readToVec3(words));
		}
		else if (words[0] == "f") {
			readFace(words, v, vt, vn, vertices, indices, meshs[currentMesh]);
		}
	}
	file.close();

	std::cout << " - " << meshs[currentMesh]._indicesCount << " triangles" << std::endl;

	// Object buffer construction
	uint32_t		buffers[3] = {0};

	glGenVertexArrays(1, &buffers[2]);
	glBindVertexArray(buffers[2]);

	glGenBuffers(1, &buffers[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);

	return (new Object(buffers, names, meshs));
}

void	Object::render(const class Shader &shader)
{
	shader.use();
	glBindVertexArray(this->_VAO);
	glDrawElements(GL_TRIANGLES, 18960, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//-void	Object::setMaterial(const struct MaterialGroup &mats)
//-{
	//-for (Mesh msh : this->_meshs)
		//-for (Material mat : mats._materials)
			//-if (msh._materialName == mat.getName())
				//-msh._materialPtr = &mat;
//-}

const std::string	& Object::getMtlName() const
{
	return (this->_mtlName);
}

vec3	readToVec3(const std::vector<std::string> &words)
{
	return ((vec3){(float)atof(words[1].c_str()),
			(float)atof(words[2].c_str()),
			(float)atof(words[3].c_str())});
}

vec2	readToVec2(const std::vector<std::string> &words)
{
	return ((vec2){(float)atof(words[1].c_str()),
			(float)atof(words[2].c_str())});
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
		vertices.push_back(0);
		vertices.push_back(0);
		vertices.push_back(0);
	}
	else {
		size_t	vnIndex = atol(spDesc[2].c_str()) - 1;

		vertices.push_back(vn[vnIndex].entries[0]);
		vertices.push_back(vn[vnIndex].entries[1]);
		vertices.push_back(vn[vnIndex].entries[2]);
	}
}
