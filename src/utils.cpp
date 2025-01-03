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
