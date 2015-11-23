#include "renderer.h"
#include <tuple>
#include <climits>

double elapsedSteps = 0;

Renderer::Renderer(int width, int height) {
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
    
    setCenterX = 0;
    setCenterY = 0;
    
    updateFunctions = new vector<void (*)(int)>();
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

void mouseWheelCallback(GLFWwindow* window, double scrollX, double scrollY) {
    elapsedSteps += scrollY;
    std::cout << scrollX << "," << scrollY << std::endl;
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
    
//    GLFWscrollfun* scrollFun = mouseWheelCallback;
    
    glfwSetScrollCallback(window, mouseWheelCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
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
    
    double mouseXD = 0;
    double mouseYD = 0;
    glfwGetCursorPos(_window, &mouseXD, &mouseYD);
    float mouseX = (float)mouseXD;
    float mouseY = (float)mouseYD;
    
    int screenWidth = 0;
    int screenHeight = 0;
    glfwGetWindowSize(_window, &screenWidth, &screenHeight);
    
    // CUBE
//    transform = glm::translate(transform, glm::vec3(0.0f,0.0f,-3.0f-sinf(elapsedSteps/50.0f)*1.0f));
//    transform = glm::rotate(transform, elapsedSteps/100.0f, glm::vec3(1.0f,1.0f,1.0f));
//    objects[0]->transform = transform;
//    glm::mat4 projection = glm::perspective(35.0f, (float)screenWidth/(float)screenHeight, 0.1f, 200.0f);
    
    // TUNNEL
//    transform = glm::translate(transform, glm::vec3(0.0f,-0.5f,0.0));
//    transform = glm::rotate(transform, elapsedSteps*(float)INT_MAX, glm::vec3(0.0f,0.0f,1.0f));
//    transform = glm::translate(transform, glm::vec3(-0.05f,5.f,-10.0f+(elapsedSteps/50.0f)));
//    objects[0]->transform = transform;
//    glm::mat4 projection = glm::perspective(35.0f, (float)screenWidth/(float)screenHeight, 0.1f, 200.0f);
    
    
    // Floating Cubes
//    for (int i = 0; i < objects.size(); i++) {
//        transform = glm::mat4();
//        transform = glm::translate(transform, glm::vec3(0.0f,0.0f,-5.0f));
//        transform = glm::rotate(transform, i*0.7f, glm::vec3(0.0f,0.0f,1.0f));
//        transform = glm::rotate(transform, -1.0f*elapsedSteps/500.0f, glm::vec3(0.0f,1.0f,0.0f));
//        transform = glm::translate(transform, glm::vec3(4.0f+periodicOffset(elapsedSteps,1.0f),0.0f,0.0f));
//        objects[i]->transform = transform;
//    }
//    glm::mat4 projection = glm::perspective(35.0f, (float)screenWidth/(float)screenHeight, 0.1f, 200.0f);
    
    // Splody Cubes
//    for (int i = 0; i < objects.size(); i++) {
//        float iterProg = 1+i/((float)objects.size());
//        transform = glm::mat4();
//
//        transform = glm::translate(transform, glm::vec3(0.0f,0.0f,-100.0f));
//        transform = glm::scale(transform, glm::vec3(10.0f));
//        transform = glm::rotate(transform, i*0.7f, glm::vec3(0.0f,0.0f,1.0f));
//        transform = glm::rotate(transform, -1.0f*iterProg*2*elapsedSteps/100.0f, glm::vec3(0.0f,1.0f,0.0f));
//        
//        transform = glm::translate(transform, glm::vec3(4.0f*iterProg+periodicOffset(elapsedSteps,1.0f),0.0f,0.0f));
//        objects[i]->transform = transform;
//    }
//    glm::mat4 projection = glm::perspective(40.0f, (float)screenWidth/(float)screenHeight, 10.0f, 200.0f);

    // Crystals
//    transform = glm::mat4();
//
//    transform = glm::translate(transform, glm::vec3(0.0f,0.0f,-20.0f));
//    transform = glm::scale(transform, glm::vec3(10.0f));
//    transform = glm::translate(transform, glm::vec3(2.0-elapsedSteps/1000.0f,10.0-elapsedSteps/100.0f,0.0f));
//    objects[0]->transform = transform;
//    glm::mat4 projection = glm::perspective(40.0f, (float)screenWidth/(float)screenHeight, 10.0f, 200.0f);
    
    // Fractals
    transform = glm::mat4();
    transform = glm::scale(transform, glm::vec3(elapsedSteps/100.0f));
    objects[0]->transform = transform;
    glm::mat4 projection = glm::perspective(40.0f, (float)screenWidth/(float)screenHeight, 10.0f, 200.0f);
//    float dMouseX = (lastMouseX - mouseX);
//    float dMouseY = (lastMouseY - mouseY);
//    if (std::abs(dMouseX) > screenWidth-20) {
//        dMouseX = 0;
//    }
//    if (std::abs(dMouseY) > screenHeight-20) {
//        dMouseY = 0;
//    }
    setCenterX += .00003 * mouseX * elapsedSteps / pow(1.006,elapsedSteps);
    setCenterY += .00003 * mouseY * elapsedSteps / pow(1.006,elapsedSteps);
//    cout <<setCenterX << "," << setCenterY << endl;
    
    // CHANGE ME STEPHEN!
    
//        setCenterX = -0.76649;
//        setCenterY = -0.107601;
//    setCenterX = 0.292641;
//    setCenterY = -0.0148172;
//        setCenterX = 0.148979;
//        setCenterY = 0.645027;
//    setCenterX = 0.0;
//    setCenterY = 0.0;
    
    // Cards
//    glm::mat4 projection = glm::perspective(35.0f, (float)screenWidth/(float)screenHeight, 0.1f, 200.0f);
    
    // Newton fractal
    
    
    // DO NOT MODIFY
    for (auto f : *updateFunctions) {
        f(elapsedSteps);
    }
    
    GLuint projection_transform_location = glGetUniformLocation(shader_program, "projection_transform");
    glUniformMatrix4fv(projection_transform_location, 1, GL_FALSE, glm::value_ptr(projection));
    
    GLuint time_loc = glGetUniformLocation(shader_program, "time");
    GLuint rc1_loc = glGetUniformLocation(shader_program, "rainbowColor1");
    GLuint rc2_loc = glGetUniformLocation(shader_program, "rainbowColor2");
    GLuint screen_size_loc = glGetUniformLocation(shader_program, "screenSize");
    GLuint mouse_pos_loc = glGetUniformLocation(shader_program, "mousePos");
    GLuint last_mouse_pos_loc = glGetUniformLocation(shader_program, "lastMousePos");
    GLuint set_center_loc = glGetUniformLocation(shader_program, "setCenter");
    
    std::tuple<float,float,float> rc1 = rainbowColorForTime(elapsedSteps);
    
    glUniform1f(time_loc, (float)elapsedSteps);
    glUniform3f(rc1_loc, std::get<0>(rc1),std::get<1>(rc1),std::get<2>(rc1));
    glUniform3f(rc2_loc, 1-std::get<0>(rc1),1-std::get<1>(rc1),1-std::get<2>(rc1));
    glUniform2f(screen_size_loc, (float)screenWidth, (float)screenHeight);
    glUniform2f(mouse_pos_loc, mouseX, mouseY);
    glUniform2f(last_mouse_pos_loc, lastMouseX, lastMouseY);
    glUniform2f(set_center_loc, setCenterX, setCenterY);
    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

void Renderer::runMainLoop(GLFWwindow* window, GLint shaderProgram) {
	while (!glfwWindowShouldClose (window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram (shaderProgram);
        
        // Add one for normal time scale, comment to stop time
//        elapsedSteps+=1;
        
        // update other events like input handling
        glfwPollEvents();
        
        populateUniforms(shaderProgram);
        for (int i = 0; i < objects.size(); i++) {
            objects[i]->render(shaderProgram);
        }
        
        // make the mouse wrap around the screen
        
        double mouseXD = 0;
        double mouseYD = 0;
        glfwGetCursorPos(_window, &mouseXD, &mouseYD);
        float x = (float)mouseXD;
        float y = (float)mouseYD;
        int w = 0;
        int h = 0;
        glfwGetWindowSize(_window, &w, &h);
        
        // Border method ------
//        int wrapBorderThickness = 5;
//        if (x > w-wrapBorderThickness) {
//            glfwSetCursorPos(_window, wrapBorderThickness+1, y);
//        }
//        if (y > h-wrapBorderThickness) {
//            glfwSetCursorPos(_window, x, wrapBorderThickness+1);
//        }
//        if (x < wrapBorderThickness) {
//            glfwSetCursorPos(_window, w-wrapBorderThickness-1, y);
//        }
//        if (y < wrapBorderThickness) {
//            glfwSetCursorPos(_window, x, h-wrapBorderThickness-1);
//        }

        
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

void Renderer::addUpdateFunction(void (*f)(int)) {
    updateFunctions->push_back(f);
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
	std::cout << "Started visualization with " << objects.size() << " objects." << std::endl;
	runMainLoop(_window, _shaderProgram);
	terminate();
	return 0;
}