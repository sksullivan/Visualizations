#include "RenderObject.h"

RenderObject::RenderObject(Geometry* geometry) {
    this->geometry = geometry;
    transform = glm::mat4();
}

void RenderObject::render(GLuint shader_program) {
    GLuint transform_location = glGetUniformLocation(shader_program, "model_transform");
    glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(transform));
    geometry->render();
}