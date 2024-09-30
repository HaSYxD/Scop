#include <iostream>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.hpp>

#define WIN_WIDTH	1600
#define WIN_HEIGHT	900

int	main()
{
	//Initialize GLFW for OpenGL
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create GLFW window
	GLFWwindow	*window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "SCOPE", NULL, NULL);
	if (!window) {
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return (EXIT_FAILURE);
	}
	//Select the OpenGL context
	glfwMakeContextCurrent(window);

	//Initialize GLAD to interface with OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		glfwTerminate();
		return (EXIT_FAILURE);
	}
	//Set The size and position of the OpenGl window inside the GLFW window
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);


	//Creating vertex Buffer
	float	vertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	unsigned int	indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int	vertexBufferObject = 0;
	glGenBuffers(1, &vertexBufferObject);		//Generating a vertex buffer object on the GPU and getting it's ID
	
	unsigned int	elementBufferObject = 0;	//Generating an element buffer object on the GPU and getting it's ID
	glGenBuffers(1, &elementBufferObject);

	unsigned int	vertexArrayObject = 0;
	glGenVertexArrays(1, &vertexArrayObject);	//Generating a vertex array object on the GPU and getting it's ID

	glBindVertexArray(vertexArrayObject);						//Bind Vertex Array object to store vertex attribute calls
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);				//Binding the vertex buffer object to the target "GL_ARRAY_BUFFER"
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//Send "vertices" into "GL_ARRAY_BUFFER" wich is bound to "vertexBufferObject"

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);			//Binding "elementBufferObject" to "GL_ELEMENT_ARRAY_BUFFER"
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//Send "indices" into "GL_ELEMENT_ARRAY_BUFFER"
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	Shader	shader("./assets/shaders/default-vert.glsl", "./assets/shaders/default-frag.glsl");
	
	//Main loop of the Program
	while (!glfwWindowShouldClose(window)) {	
		//Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();				//Choosing shaders to render with
		glBindVertexArray(vertexArrayObject);	//Choosing vertexArrayObject to render with
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//Draw call
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		
		//GLFW input handling
		glfwPollEvents();
		
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
			break ;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			shader.recompile();
	}
	glfwTerminate();
	return (EXIT_SUCCESS);
}
