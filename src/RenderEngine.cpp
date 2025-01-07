# include <cmath>

# include <Shader.hpp>
# include <Object.hpp>
# include <Material.hpp>
# include <RenderEngine.hpp>
# include <math.hpp>
# include <utils.hpp>

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

void	RenderEngine::run(const std::string &path)
{
	Object	* obj = Object::load(path);

	std::vector<std::string>	spStr = split(path, '/');

	MaterialGroup	* mats = Material::loadMtlFile(join(spStr, "/", spStr.size() - 1) + obj->getMtlName());
	obj->setMaterial(mats);
	
	float	time = 0;
	float	scale = 1;
	vec3	a = vec3{0, 0, 0};
	vec3	p = vec3{0, 0, 0};
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
		vec3	objCenter = obj->getCenter();

		model = matrixMultiply(model, translationMatrix(objCenter));

		model = matrixMultiply(model, scaleMatrix(vec3{scale, scale, scale}));
		model = matrixMultiply(model, rotationMatrix(a));
		model = matrixMultiply(model, translationMatrix(p));

		model = matrixMultiply(model, translationMatrix(vec3{
					-objCenter.entries[0], -objCenter.entries[1], -objCenter.entries[2]}));
		this->_shaders[0]->setMat4("model", model);
		
		if (glfwGetKey(this->_window, GLFW_KEY_U) == GLFW_PRESS)
			p.entries[0] += 0.01;
		else if (glfwGetKey(this->_window, GLFW_KEY_J) == GLFW_PRESS)
			p.entries[0] -= 0.01;

		if (glfwGetKey(this->_window, GLFW_KEY_I) == GLFW_PRESS)
			p.entries[1] += 0.01;
		else if (glfwGetKey(this->_window, GLFW_KEY_K) == GLFW_PRESS)
			p.entries[1] -= 0.01;
		
		if (glfwGetKey(this->_window, GLFW_KEY_O) == GLFW_PRESS)
			p.entries[2] += 0.01;
		else if (glfwGetKey(this->_window, GLFW_KEY_L) == GLFW_PRESS)
			p.entries[2] -= 0.01;
		
		if (glfwGetKey(this->_window, GLFW_KEY_Q) == GLFW_PRESS)
			a.entries[0] += 0.01;
		else if (glfwGetKey(this->_window, GLFW_KEY_A) == GLFW_PRESS)
			a.entries[0] -= 0.01;

		if (glfwGetKey(this->_window, GLFW_KEY_W) == GLFW_PRESS)
			a.entries[1] += 0.01;
		else if (glfwGetKey(this->_window, GLFW_KEY_S) == GLFW_PRESS)
			a.entries[1] -= 0.01;
		
		if (glfwGetKey(this->_window, GLFW_KEY_E) == GLFW_PRESS)
			a.entries[2] += 0.01;
		else if (glfwGetKey(this->_window, GLFW_KEY_D) == GLFW_PRESS)
			a.entries[2] -= 0.01;
		
		if (glfwGetKey(this->_window, GLFW_KEY_UP) == GLFW_PRESS)
			scale += 0.01;
		else if (glfwGetKey(this->_window, GLFW_KEY_DOWN) == GLFW_PRESS)
			scale -= 0.01;

		if (scale < MIN_SCALE) scale = MIN_SCALE;

		mat4	view = viewMatrix(
				this->_camera.getPostion(),
				this->_camera.getTarget());
		this->_shaders[0]->setMat4("view", view);
		
		vec3	lPos = {sinf(time * 0.1) * 100, cosf(time * 0.1) * 100, 100.0f};
		this->_shaders[0]->setVec3("lPos", lPos);

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
