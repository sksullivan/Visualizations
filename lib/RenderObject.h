#import "../common.h"
#import "geometry.h"

class RenderObject {
public:
    RenderObject(Geometry* geometry);
    void render(GLuint shader_program);
    Geometry* geometry;
    glm::mat4 transform;
};