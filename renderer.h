#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

class Renderer {
private:
	GLFWwindow* setupWindow();
	void setupRenderer();
	GLuint setupGeometry();
	GLint setupShaders();
	void runMainLoop(GLFWwindow*, GLint, GLuint);
	void terminate();
	char* loadShader(std::string);

	GLFWwindow* _window;
	GLint _shaderProgram;
	GLuint _vao;
public:
	Renderer();
	int begin();
};