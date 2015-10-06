#version 140

in mat4 transform;
in vec3 position;
 
void main(void) {
    float timer = -0.1;
    mat3 scale = mat3(
                      vec3(0.1,  0.0,  0.0),
                      vec3(0.0,  0.1,  0.0),
                      vec3(0.0,  0.0,  0.1)
                      );
    
    mat3 rotation = mat3(
                         vec3( cos(timer),  sin(timer),  0.0),
                         vec3(-sin(timer),  cos(timer),  0.0),
                         vec3(        0.0,         0.0,  1.0)
                         );
	gl_Position = vec4(scale*position, 1.0);
}