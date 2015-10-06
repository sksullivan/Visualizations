#include "lib/objModel.h" 
#include <glm/gtc/matrix_transform.hpp>

#include "common.h"


class Renderer {
private:
	GLFWwindow* setupWindow(int width, int height);
	void setupRenderer();
	void setupGeometry();
	GLint setupShaders();
	void runMainLoop(GLFWwindow*, GLint);
	void terminate();
	char* loadShader(std::string);
    void populateUniforms(GLuint shaderProgram);

	GLFWwindow* _window;
	GLint _shaderProgram;
	GLuint _vao;
    
    std::vector<CObjModel*> models;
public:
	Renderer();
	int begin(int width, int height);
    
    struct {
        GLuint transform;
    } uniforms;
};