# include <fstream>
# include <sstream>
# include <iostream>
# include <cstdlib>
# include <string>

# include <Image.hpp>

Image::Image(const int &width, const int &height, const uint8_t *imgData)
{
	(void)width;
	(void)height;
	(void)imgData;
}

Image::~Image() {}

Image	Image::load(const std::string &path)
{
	std::ifstream	file(path.c_str(), std::fstream::binary);

	if (!file.is_open())
		throw (std::runtime_error("could not open .ppm file"));

	std::string	headerDataBuffer[3];

	for (int i = 0; i < 3; i++)
		std::getline(file, headerDataBuffer[i]);

	if (headerDataBuffer[0] != "P6")
		throw (std::runtime_error("invalid or corrupted file"));

	int	width = atoi(headerDataBuffer[1].c_str());
	int	whiteSpace = headerDataBuffer[1].find(' ');
	int	height = atoi(headerDataBuffer[1].c_str() + whiteSpace);

	if ((width == 0 && headerDataBuffer[1][0] != 0)
			|| (height == 0 && headerDataBuffer[1][whiteSpace + 1] != 0))
		throw (std::runtime_error("invalid or corrupted file"));

	if (width < 0 || height < 0)
		throw (std::runtime_error("invalid or corrupted file"));
	
	int	maxColValue = atoi(headerDataBuffer[2].c_str());

	if ((maxColValue == 0 && headerDataBuffer[2][0] != 0) 
			|| (maxColValue < 0 || maxColValue > 255))
		throw (std::runtime_error("invalid or corrupted file"));
	
	if (width > 1024 || height > 1024)
		throw (std::runtime_error("image is to large"));

	char	* imgData = new char[width * height];

	file.read(imgData, width * height);
	file.close();
	return (Image(width, height, (uint8_t *)imgData));
}
		
unsigned int	& Image::getWidth()
{
	return (this->_width);
}

unsigned int	& Image::getHeight()
{
	return (this->_height);
}

uint8_t		* Image::getData()
{
	return (this->_data);
}
