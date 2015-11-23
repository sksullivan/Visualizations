#version 140

in vec3 vertex_position;
in vec3 vertex_color;
in vec2 aTexCoord;
out vec2 vTexCoord;


void main(void) {
    vTexCoord = aTexCoord;
    gl_Position = vec4(vertex_position, 1.0);
}