# pragma once

# include <iostream>

//namespace COLOR {
# define BLACK		"\033[0;30m"
# define RED		"\033[0;31m"
# define GREEN		"\033[0;32m"
# define YELLOW		"\033[0;33m"
# define BLUE		"\033[0;34m"
# define MAGENTA	"\033[0;35m"
# define CYAN		"\033[0;36m"
# define WHITE		"\033[0;37m"
# define DEFAULT	"\033[0;39m"
# define RESET		"\033[0;0m"
# define GRAY		"\033[0;38;5;250m"

# define BOLD_BLACK	"\033[1;30m"
# define BOLD_RED	"\033[1;31m"
# define BOLD_GREEN	"\033[1;32m"
# define BOLD_YELLOW	"\033[1;33m"
# define BOLD_BLUE	"\033[1;34m"
# define BOLD_MAGENTA	"\033[1;35m"
# define BOLD_CYAN	"\033[1;36m"
# define BOLD_WHITE	"\033[1;37m"
# define BOLD_DEFAULT	"\033[1;39m"
# define BOLD_GRAY	"\033[1;38;5;250m"
//};

namespace LEVEL {
enum	LEVEL {
	LOG     = 0,
	INFO    = 1,
	WARNING = 2,
	ERROR   = 3
};
};

void	print(const std::string str, const unsigned int lvl);
