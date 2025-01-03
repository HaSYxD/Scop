# include <cstdlib>

# include <Shader.hpp>
# include <RenderEngine.hpp>
# include <Image.hpp>
# include <IOHandling.hpp>
# include <utils.hpp>

float	gXOffset = 0, gYOffset = 0;

void	handleMouseScroll(GLFWwindow *window, double xOff, double yOff)
{
	(void)window;
	gXOffset = (float)xOff;
	gYOffset = (float)yOff;
}

int	main()
{
	try
	{
		RenderEngine	re = RenderEngine();

		glfwSetScrollCallback(re.getWindow(), &handleMouseScroll);
		re.run();
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
