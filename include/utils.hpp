# pragma once

# include <vector>
# include <string>

# include <math.hpp>

std::vector<std::string>	split(const std::string &str, const char &c);
std::string			join(const std::vector<std::string> &words, const std::string &del, const size_t &n);

vec3	readToVec3(const std::vector<std::string> &words);
vec2	readToVec2(const std::vector<std::string> &words);
