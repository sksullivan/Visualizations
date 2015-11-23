#import "lib/geometry.h"
#import "lib/RenderObject.h"
#import <glm/gtc/matrix_transform.hpp>

#import "common.h"
#import "lib/utils.h"

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
//    float elapsedSteps;
    
    std::vector<RenderObject*> objects;
    std::string vertexShaderPath, fragmentShaderPath;
    
    float lastMouseX, lastMouseY;
    float setCenterX, setCenterY;
    
//    void mouseWheelCallback(GLFWwindow* window, double scrollX, double scrollY);
    
    std::vector<void (*)(int)>* updateFunctions;
public:
	Renderer(int width, int height);
    void addRenderObject(RenderObject* renderObject);
    void setShaders(std::string vertPath, std::string fragPath);
	int begin();
    void addUpdateFunction(void (*f)(int));
    
    struct {
        GLuint transform;
    } uniforms;
};