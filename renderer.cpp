#include "renderer.h"

Renderer::Renderer() {

}

char* Renderer::loadShader (std::string filename) {
	std::ifstream in(filename);
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	in.close();
  // cout << contents;
	char* ret = (char*)malloc(sizeof(char)*contents.length());
	strcpy(ret,contents.c_str());
	return ret;
}

GLFWwindow* Renderer::setupWindow() {
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

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
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

GLuint Renderer::setupGeometry() {
	float points[] = {
		0.0f,  0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
	};

	GLuint vbo = 0;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	return vao;
}

GLint Renderer::setupShaders() {
	char* vertex_shader = loadShader("vert.glsl");
	char* fragment_shader = loadShader("frag.glsl");

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

	free(vertex_shader);
	free(fragment_shader);
	return shader_programme;
}

void Renderer::runMainLoop(GLFWwindow* window, GLint shaderProgram, GLuint vao) {
	while (!glfwWindowShouldClose (window)) {
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram (shaderProgram);
		glBindVertexArray (vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays (GL_TRIANGLES, 0, 3);
		// update other events like input handling 
		glfwPollEvents ();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (window);
	}
}

void Renderer::terminate() {
	// close GL context and any other GLFW resources
	glfwTerminate();
}

int Renderer::begin () {
	_window = setupWindow();
	if (!_window) {
		std::cout << "Falied to initialize glfw window." << std::endl;
		return 1;
	}
	setupRenderer();
	_vao = setupGeometry();
	if (!_vao) {
		std::cout << "Falied to initialize gemoetry." << std::endl;
		return 1;
	}
	_shaderProgram = setupShaders();
	if (!_shaderProgram) {
		std::cout << "Falied to initialize shaders." << std::endl;
		return 1;
	}
	std::cout << "Started visualization." << std::endl;
	runMainLoop(_window, _shaderProgram, _vao);
	terminate();
	return 0;
}