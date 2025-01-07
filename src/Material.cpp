# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <utils.hpp>
# include <Material.hpp>
# include <Image.hpp>

# include <fstream>
# include <iostream>

Material::Material(const std::string &name) : _name(name)
{
	this->_ambColor = vec3{1, 1, 1};
	this->_diffColor = vec3{1, 1, 1};
	this->_specColor = vec3{1, 1, 1};
	this->_specExponent = 0;
	this->_texture = 0;
}

Material::~Material() {}

MaterialGroup	* Material::loadMtlFile(const std::string &path)
{
	std::ifstream	file(path);

	if (!file.is_open())
		throw (std::runtime_error("could not open .mtl file"));

	MaterialGroup	*mtlg = new MaterialGroup;

	// isolate the file name and set it in the MaterialGroup
	std::vector<std::string>	spStr = split(path, '/');

	mtlg->_mtlFileName = spStr[spStr.size() - 1];
	mtlg->_materials.push_back(Material("default name"));

	// start to read the file line by line
	std::string	line = "";
	uint32_t	currMat = 0;;

	while (std::getline(file, line)) {
		spStr = split(line, ' ');

		if (!spStr.size())
			continue ;
		else if (spStr[0] == "newmtl") {
			if (mtlg->_materials[currMat]._name == "default name") {
				mtlg->_materials[currMat]._name = spStr[1];
				continue ;
			}
			mtlg->_materials.push_back(Material(spStr[1]));
			currMat++;
		}
		else if (spStr[0] == "Ka")
			mtlg->_materials[currMat]._ambColor = readToVec3(spStr);
		else if (spStr[0] == "Kd")
			mtlg->_materials[currMat]._diffColor = readToVec3(spStr);
		else if (spStr[0] == "Ks")
			mtlg->_materials[currMat]._specColor = readToVec3(spStr);
		else if (spStr[0] == "Ns") {
			mtlg->_materials[currMat]._specExponent = atof(spStr[1].c_str());
			std::cout << spStr[1] << std::endl;
		}
		else if (spStr[0] == "map_Ka" || spStr[0] == "map_Kd" || spStr[0] == "map_Ks"){
			if (mtlg->_materials[currMat]._texture) continue ;

			std::string	texPath(trim(path, '/') + "/" + spStr[1]);
			std::cout << texPath << std::endl;
			Image	* img = Image::load(texPath);

			glCreateTextures(GL_TEXTURE_2D, 1, &mtlg->_materials[currMat]._texture);
			glBindTexture(GL_TEXTURE_2D, mtlg->_materials[currMat]._texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->getWidth(), img->getHeight(),
					0, GL_RGB, GL_UNSIGNED_BYTE, img->getData());
			delete img;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//-glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	return (mtlg);
}

std::string	& Material::getName()
{
	return (this->_name);
}

uint32_t	& Material:: getTexture()
{
	return (this->_texture);
}

vec3	& Material::getAmbColor()
{
	return (this->_ambColor);
}

vec3	& Material::getDiffColor()
{
	return (this->_diffColor);
}

vec3	& Material::getSpecColor()
{
	return (this->_specColor);
}

float	& Material::getSpecExponent()
{
	return (this->_specExponent);
}
