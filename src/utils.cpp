# include <sstream>
# include <utils.hpp>
# include <iostream>

std::vector<std::string>	split(const std::string &str, const char &c)
{
	std::vector<std::string>	words;
	std::string		buff;
	std::stringstream	ss(str);

	while (std::getline(ss >> std::ws, buff, c))
		words.push_back(buff);
	return (words);
}

vec3	readToVec3(const std::vector<std::string> &words)
{
	return (vec3{(float)atof(words[1].c_str()),
			(float)atof(words[2].c_str()),
			(float)atof(words[3].c_str())});
}

vec2	readToVec2(const std::vector<std::string> &words)
{
	return ((vec2){(float)atof(words[1].c_str()),
			(float)atof(words[2].c_str())});
}

std::string	join(const std::vector<std::string> &words, const std::string &del, const size_t &n)
{
	std::string	str = "";

	for (size_t i = 0; i < n; i++) {
		str +=words[i];
		str +=del;
	}
	return (str);
}

std::string	trim(const std::string &str, const char &c)
{
	size_t		pos = str.find_last_of(c);

	return (str.substr(0, pos));
}
