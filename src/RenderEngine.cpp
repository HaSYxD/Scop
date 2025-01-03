# include <cmath>

# include <Shader.hpp>
# include <Object.hpp>
# include <Material.hpp>
# include <RenderEngine.hpp>
# include <math.hpp>

extern float	gXOffset, gYOffset;

RenderEngine::RenderEngine() : _camera((vec3){0, 0, 0})
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
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CW);

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
	Object	* obj = Object::load("./assets/models/campfire_complete.obj");
	// Object	* obj = Object::load("./assets/models/teapot2.obj");
	//-MaterialGroup	mats = loadMtlFile(obj.getMtlName());
	//-obj.setMaterial(mats);


	float	time = 0;
	int	hasBeenPressed = 0;

	mat4	projection = projectionMatrix(90.0f, (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);
	this->_shaders[0]->setMat4("projection", projection);

	//Main loop of the Program
	while (!glfwWindowShouldClose(this->_window)) {	
		//Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->_shaders[0]->use();				//Choosing shaders to render with
		this->_shaders[0]->setFloat("time", time);				//Choosing shaders to render with
	
		this->_camera.update((vec2){gXOffset, gYOffset});

		// Plane model matrix
		mat4	model = identityMatrix();
		model = scaleMatrix(model, (vec3){1.0, 1.0, 1.0});
		//-model = rotationMatrix(model, (vec3){time, time, time});
		model = translationMatrix(model, (vec3){0.0f, 0.0f, 0.0f});
		this->_shaders[0]->setMat4("model", model);
		
		/*if (glfwGetKey(this->_window, GLFW_KEY_LEFT) == GLFW_PRESS)
			a += 0.01;
		else if (glfwGetKey(this->_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			a -= 0.01;

		if (glfwGetKey(this->_window, GLFW_KEY_UP) == GLFW_PRESS)
			z += 0.01;
		else if (glfwGetKey(this->_window, GLFW_KEY_DOWN) == GLFW_PRESS)
			z -= 0.01;
		*/

		mat4	view = viewMatrix(
				this->_camera.getPostion(),
				this->_camera.getTarget());
		this->_shaders[0]->setMat4("view", view);
		
		vec3	lPos = {sinf(time) * 10, cosf(time) * 10, -2.0f};
		this->_shaders[0]->setVec3("lPos", lPos);
		
		//-glBindVertexArray(vertexArrayObject);	//Choosing vertexArrayObject to render with
		//-glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//Draw call
		//-glBindVertexArray(0);

		obj->render(*this->_shaders[0]);
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
}

GLFWwindow	* RenderEngine::getWindow()
{
	return (this->_window);
}
