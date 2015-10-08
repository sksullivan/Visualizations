#include "renderer.h"
#include <tuple>

Renderer::Renderer(int width, int height) {
    elapsedSteps = 0;
    if (setupGL() == 1) {
        exit(1);
    }
    _window = setupWindow(width,height);
    if (!_window) {
        std::cout << "ERROR: Falied to initialize glfw window." << std::endl;
        exit(1);
    }
    
    vertexShaderPath = "";
    fragmentShaderPath = "";
}

int Renderer::setupGL() {
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return 0;
}

GLFWwindow* Renderer::setupWindow(int width, int height) {
	GLFWwindow* window = glfwCreateWindow(width, height, "Seeing is Believeing ", NULL, NULL);
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

void Renderer::setShaders(std::string vertPath, std::string fragPath) {
    vertexShaderPath = vertPath;
    fragmentShaderPath = fragPath;
}

GLint Renderer::setupShaders() {
	char* vertex_shader = loadShader(vertexShaderPath);
	char* fragment_shader = loadShader(fragmentShaderPath);

	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader, NULL);
	glCompileShader (vs);

	GLint compileStatus;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus != GL_TRUE) {
		std::cout << "ERROR: Vertex shader failed to compile." << std::endl;
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
		std::cout << "ERROR: Fragment shader failed to compile." << std::endl;
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
    
    glBindAttribLocation (shader_programme, 0, "vertex_position");
    glBindAttribLocation (shader_programme, 1, "vertex_color");
    
	glLinkProgram (shader_programme);

	GLint linkStatus;
	glGetProgramiv(shader_programme, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		std::cout << "ERROR: Shader program failed to link." << std::endl;
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

void Renderer::populateUniforms(GLuint shader_program) {
    glm::mat4 transform = glm::mat4();
    transform = glm::rotate(transform, 45.0f+elapsedSteps/100.0f, glm::vec3(0.0f,0.0f,1.0f));
    transform = glm::scale(transform, glm::vec3(0.02f));
    glm::mat4 transform0 = glm::translate(transform, glm::vec3(0.0f,6.0f,-1.0f));
    objects[0]->transform = transform0;
    
    GLuint projection_transform_location = glGetUniformLocation(shader_program, "projection_transform");
    glm::mat4 projection = glm::perspective(33.0f, 1.0f, 0.1f, 200.0f);
    glUniformMatrix4fv(projection_transform_location, 1, GL_FALSE, glm::value_ptr(projection));
    
    GLuint time_loc = glGetUniformLocation(shader_program, "time");
    GLuint rc1_loc = glGetUniformLocation(shader_program, "rainbowColor1");
    GLuint rc2_loc = glGetUniformLocation(shader_program, "rainbowColor2");
    GLuint screen_size_loc = glGetUniformLocation(shader_program, "screenSize");
    
    std::tuple<float,float,float> rc1 = rainbowColorForTime(elapsedSteps);
    int screenWidth = 0;
    int screenHeight = 0;
    glfwGetWindowSize(_window, &screenWidth, &screenHeight);
    
    glUniform1f(time_loc, (float)elapsedSteps);
    glUniform3f(rc1_loc, std::get<0>(rc1),std::get<1>(rc1),std::get<2>(rc1));
    glUniform3f(rc2_loc, 1-std::get<0>(rc1),1-std::get<1>(rc1),1-std::get<2>(rc1));
    glUniform2f(screen_size_loc, (float)screenWidth, (float)screenHeight);
}

void Renderer::runMainLoop(GLFWwindow* window, GLint shaderProgram) {
	while (!glfwWindowShouldClose (window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram (shaderProgram);
        elapsedSteps+=1;
        populateUniforms(shaderProgram);
        for (int i = 0; i < objects.size(); i++) {
            objects[i]->render(shaderProgram);
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

void Renderer::addRenderObject(RenderObject* renderObject) {
    objects.push_back(renderObject);
    renderObject->geometry->bufferDataToGPU();
}

int Renderer::begin() {
	setupRenderer();
    if (fragmentShaderPath == "" || vertexShaderPath == "") {
        std::cout << "WARNING: no shader loaded..." << std::endl;
    }
    if (objects.size() == 0) {
        std::cout << "WARNING: no objects loaded..." << std::endl;
    }
	_shaderProgram = setupShaders();
	if (!_shaderProgram) {
		std::cout << "ERROR: Falied to initialize shaders." << std::endl;
		return 1;
	}
	std::cout << "Started visualization." << std::endl;
	runMainLoop(_window, _shaderProgram);
	terminate();
	return 0;
}