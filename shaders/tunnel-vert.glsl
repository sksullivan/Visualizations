#version 140

uniform mat4 model_transform;
uniform mat4 projection_transform;
in vec3 vertex_position;
in vec3 vertex_color;

out vec3 color;
 
void main(void) {
    color = vertex_color;
	gl_Position = projection_transform*model_transform*vec4(vertex_position, 1.0);
}