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
}

Material::~Material() {}

MaterialGroup	* Material::loadMtlFile(const std::string &path)
{
	std::cout << path << std::endl;

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
		else if (spStr[0] == "Ns")
			mtlg->_materials[currMat]._specExponent = atof(spStr[1].c_str());
		else if (spStr[0] == "map_Ka" || spStr[0] == "map_Kd" || spStr[0] == "map_Ks"){
			Image	img = Image::load(trim(path, '/') + spStr[1]);
		}
	}

	for (Material &mat : mtlg->_materials) {
		std::cout << mat._name << std::endl;
		std::cout << mat._ambColor.entries[0] << " | "
			<< mat._ambColor.entries[1] << " | "
			<< mat._ambColor.entries[2] << std::endl;

		std::cout << mat._diffColor.entries[0] << " | "
			<< mat._diffColor.entries[1] << " | "
			<< mat._diffColor.entries[2] << std::endl;

		std::cout << mat._specColor.entries[0] << " | "
			<< mat._specColor.entries[1] << " | "
			<< mat._specColor.entries[2] << std::endl;

		std::cout << mat._specExponent << std::endl;
	}

	return (mtlg);
}

std::string	& Material::getName()
{
	return (this->_name);
}

uintptr_t	& Material:: getTexture()
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
