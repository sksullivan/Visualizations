#include "lib/geometry.h"
#include "lib/RenderObject.h"
#include <glm/gtc/matrix_transform.hpp>

#include "common.h"
#include "lib/utils.h"


class Renderer {
private:
	GLFWwindow* setupWindow(int width, int height);
	void setupRenderer();
	GLint setupShaders();
	void runMainLoop(GLFWwindow*, GLint);
	void terminate();
    void populateUniforms(GLuint shader_program);
    int setupGL();

	GLFWwindow* _window;
	GLint _shaderProgram;
	GLuint _vao;
    float elapsedSteps;
    
    std::vector<RenderObject*> objects;
    std::string vertexShaderPath, fragmentShaderPath;
public:
	Renderer(int width, int height);
    void addRenderObject(RenderObject* renderObject);
    void setShaders(std::string vertPath, std::string fragPath);
	int begin();
    
    
    struct {
        GLuint transform;
    } uniforms;
};