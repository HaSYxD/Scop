# pragma once

# include <vector>
# include <glad/glad.h>
# include <GLFW/glfw3.h>

#define WIN_WIDTH	1600
#define WIN_HEIGHT	900

class	RenderEngine
{
	public:
		RenderEngine();
		~RenderEngine();

		void	run();
	
	private:
		std::vector<class Shader *>	_shaders;
		GLFWwindow			*_window;
};
