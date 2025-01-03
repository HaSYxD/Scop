# include <IOHandling.hpp>

static void	printLog(const std::string &str)
{
	std::cerr << DEFAULT << "[" << BOLD_GRAY << "LOG" << DEFAULT << "]: "
		<< str << RESET << std::endl;
}

static void	printInfo(const std::string &str)
{
	std::cerr << DEFAULT << "[" << CYAN << "INFO" << DEFAULT << "]: "
		<< str << RESET << std::endl;
}

static void	printWarning(const std::string &str)
{
	std::cout << BOLD_DEFAULT << "[" << BOLD_YELLOW << "WARNING" << BOLD_DEFAULT << "]: "
		<< str << RESET << std::endl;
}

static void	printError(const std::string &str)
{
	std::cerr << BOLD_DEFAULT << "[" << BOLD_RED << "ERROR" << BOLD_DEFAULT << "]: "
		<< str << RESET << std::endl;
}

void	print(const std::string str, const unsigned int lvl)
{
	switch (lvl) {
		case LEVEL::LOG:
			printLog(str);
			break ;
		case LEVEL::INFO:
			printInfo(str);
			break ;
		case LEVEL::WARNING:
			printWarning(str);
			break ;
		case LEVEL::ERROR:
			printError(str);
			break ;
		default:
			print("log level does not exist", LEVEL::ERROR);
			break ;
	};
}
