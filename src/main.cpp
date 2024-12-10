#include <cstdlib>
#include <Shader.hpp>
# include <RenderEngine.hpp>

int	main()
{
	try
	{
		RenderEngine	re = RenderEngine();
		re.run();
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
