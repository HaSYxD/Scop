# pragma once

# include <vector>
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <string>

# include <Camera.hpp>

# define WIN_WIDTH	1600
# define WIN_HEIGHT	900
# define MIN_SCALE	0.1

class	RenderEngine
{
	public:
		RenderEngine();
		~RenderEngine();

		void	run(const std::string &path);

		GLFWwindow	* getWindow();
	
	private:
		std::vector<class Shader *>	_shaders;
		GLFWwindow			*_window;
		Camera				_camera;
};
