#include "renderer.h"


Renderer::Renderer() {

}

char* Renderer::loadShader (std::string filename) {
	std::ifstream in(filename);
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	in.close();
	char* ret = (char*)malloc(sizeof(char)*contents.length());
	strcpy(ret,contents.c_str());
	return ret;
}

GLFWwindow* Renderer::setupWindow(int width, int height) {
  // start GL context and O/S window using the GLFW helper library
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return NULL;
	} 

	// uncomment these lines if on Apple OS X
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf (stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent (window);

  // start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
	return window;
}

void Renderer::setupRenderer() {
// get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
}

void Renderer::setupGeometry() { 
    CObjModel* model = new CObjModel();
    models.push_back(model);
	model->loadModel("models/cube_2.obj");
	model->bufferDataToGPU();
}

GLint Renderer::setupShaders() {
	char* vertex_shader = loadShader("shaders/vert.glsl");
	char* fragment_shader = loadShader("shaders/frag.glsl");

	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader, NULL);
	glCompileShader (vs);

	GLint compileStatus;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != GL_TRUE) {
		std::cout << "vtx shader failed to compile" << std::endl;
		GLint infoLoglength;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLoglength);
		GLchar* infoLog = new GLchar[infoLoglength + 1];
		glGetShaderInfoLog(vs, infoLoglength, NULL, infoLog);
		std::cout << infoLog << std::endl;
		delete infoLog;
		return -1;
	}

	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader, NULL);
	glCompileShader (fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		std::cout << "fgmt shader failed to compile" << std::endl;
		GLint infoLoglength;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infoLoglength);
		GLchar* infoLog = new GLchar[infoLoglength + 1];
		glGetShaderInfoLog(fs, infoLoglength, NULL, infoLog);
		std::cout << infoLog << std::endl;
		delete infoLog;
		return -1;
	}

	GLuint shader_programme = glCreateProgram ();
	glAttachShader (shader_programme, fs);
	glAttachShader (shader_programme, vs);
	glLinkProgram (shader_programme);

	GLint linkStatus;
	glGetProgramiv(shader_programme, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		std::cout << "shader program failed to link" << std::endl;
		GLint infoLoglength;
		glGetProgramiv(shader_programme, GL_INFO_LOG_LENGTH, &infoLoglength);
		GLchar* infoLog = new GLchar[infoLoglength + 1];
		glGetProgramInfoLog(shader_programme, infoLoglength, NULL, infoLog);
		std::cout << infoLog << std::endl;
		delete infoLog;
		return -1;
	}
    
    uniforms.transform = glGetUniformLocation(shader_programme, "transform");

	free(vertex_shader);
	free(fragment_shader);
	return shader_programme;
}

void Renderer::populateUniforms(GLuint program) {
//    glm::mat4 projection = glm::perspective(35.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 transform = glm::mat4(1.);
    glm::rotate(transform, 45.0f, glm::vec3(1.0f,0.0f,0.0f));
    glUniformMatrix4fv(uniforms.transform, 1, GL_FALSE, glm::value_ptr(transform));
}

void Renderer::runMainLoop(GLFWwindow* window, GLint shaderProgram) {
	while (!glfwWindowShouldClose (window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram (shaderProgram);
        populateUniforms(shaderProgram);
        for (int i = 0; i < models.size(); i++) {
            models[i]->render();
        }
		// update other events like input handling 
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}
}

void Renderer::terminate() {
	// close GL context and any other GLFW resources
	glfwTerminate();
}

int Renderer::begin (int width, int height) {
	_window = setupWindow(width,height);
	if (!_window) {
		std::cout << "Falied to initialize glfw window." << std::endl;
		return 1;
	}
	setupRenderer();
	setupGeometry();
	_shaderProgram = setupShaders();
	if (!_shaderProgram) {
		std::cout << "Falied to initialize shaders." << std::endl;
		return 1;
	}
	std::cout << "Started visualization." << std::endl;
	runMainLoop(_window, _shaderProgram);
	terminate();
	return 0;
}