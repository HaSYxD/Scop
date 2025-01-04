# include <utils.hpp>

std::vector<std::string>	split(const std::string &str, const char &c)
{
	std::vector<std::string>	words;
	size_t	i = 0;
	size_t	j = 0;

	while (i <= str.length()) {
		i = str.find(c, i);
		if (i == std::string::npos)
			i = str.length();
		i++;
		words.push_back(str.substr(j, (i - 1) - j));
		j = i;
	}
	return (words);
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

std::string	join(const std::vector<std::string> &words, const std::string &del, const size_t &n)
{
	std::string	str = "";
	for (size_t i = 0; i < n; i++) {
		str +=words[i];
		str +=del;
	}
	return (str);
}
