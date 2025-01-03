# pragma once

# include <vector>
# include <glad/glad.h>
# include <GLFW/glfw3.h>

# include <Camera.hpp>

#define WIN_WIDTH	1600
#define WIN_HEIGHT	900

class	RenderEngine
{
	public:
		RenderEngine();
		~RenderEngine();

		void	run();

		GLFWwindow	* getWindow();
	
	private:
		std::vector<class Shader *>	_shaders;
		GLFWwindow			*_window;
		Camera				_camera;
};
