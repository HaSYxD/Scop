# include <Shader.hpp>
# include <RenderEngine.hpp>
# include <math.hpp>

RenderEngine::RenderEngine()
{
	//Initialize GLFW for OpenGL
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create GLFW window
	this->_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "SCOPE", NULL, NULL);
	if (!this->_window) {
		glfwTerminate();
		throw (std::runtime_error("Failed to create GLFW window!"));
	}
	//Select the OpenGL context
	glfwMakeContextCurrent(this->_window);
	
	//Initialize GLAD to interface with OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwDestroyWindow(this->_window);
		glfwTerminate();
		throw (std::runtime_error("Failed to initialize GLAD!"));
	}
	//Set The size and position of the OpenGl window inside the GLFW window
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	this->_shaders.push_back(new Shader("./assets/shaders/default-vert.glsl", "./assets/shaders/default-frag.glsl"));
}

RenderEngine::~RenderEngine()
{
	for  (size_t i = 0; i < this->_shaders.size(); i++)
		delete this->_shaders[i];
	this->_shaders.clear();
	glfwTerminate();
}

void	RenderEngine::run()
{
	//Creating vertex Buffer
	float	vertices[] = {
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

		 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f
	};

	unsigned int	indices[] = {
		0, 1, 3,
		1, 2, 3,
		
		4, 5, 7,
		5, 6, 7,

		0, 4, 1,
		1, 4, 5,

		2, 7, 3,
		3, 7, 6
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
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);

	float	time = 0;
	int	hasBeenPressed = 0;

	//Main loop of the Program
	while (!glfwWindowShouldClose(this->_window)) {	
		//Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		this->_shaders[0]->use();				//Choosing shaders to render with
		
		this->_shaders[0]->setFloat("time", time);				//Choosing shaders to render with
		
		mat4	model = identityMatrix();
		model = scaleMatrix(model, (vec3){0.5, 0.5, 0.5});
		model = rotationMatrix(model, (vec3){time, time, time});
		model = translationMatrix(model, (vec3){0});
		this->_shaders[0]->setMat4("model", model);


		glBindVertexArray(vertexArrayObject);	//Choosing vertexArrayObject to render with
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//Draw call
		glBindVertexArray(0);

		//GLFW input handling
		glfwPollEvents();
		
		if (glfwGetKey(this->_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwDestroyWindow(this->_window);
			break ;
		}
		if (glfwGetKey(this->_window, GLFW_KEY_ENTER) == GLFW_PRESS && !hasBeenPressed) {
			std::cout << "recompiling.." << std::endl;
			this->_shaders[0]->recompile();
			hasBeenPressed = 1;
		}
		else if (glfwGetKey(this->_window, GLFW_KEY_ENTER) == GLFW_RELEASE && hasBeenPressed)
			hasBeenPressed = 0;

		glfwSwapBuffers(this->_window);
		time += 0.01;
	}
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteBuffers(1, &elementBufferObject);
}
