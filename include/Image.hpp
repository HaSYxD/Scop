# pragma once

# include <cstdint>
# include <iostream>

class Image
{
	public:
		static Image	load(const std::string &path);
		unsigned int	& getWidth();
		unsigned int	& getHeight();
		uint8_t		* getData();
	
	private:
		Image(const int &width, const int &height, const uint8_t *imgData);
		~Image();

		unsigned int	_width;
		unsigned int	_height;
		uint8_t		* _data;
};
