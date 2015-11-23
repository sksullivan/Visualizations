#version 140

uniform vec2 mousePos;
uniform vec2 lastMousePos;
uniform vec2 setCenter;
uniform vec2 screenSize;
uniform float time;

in vec2 vTexCoord;
out vec4 FragColor;

void main(void) {
    int r = 10;
    float x = gl_FragCoord.x/2.0;
    float y = (screenSize.y - gl_FragCoord.y)/2.0;
    
    if (x+r > mousePos.x && x-r < mousePos.x && y+r > mousePos.y && y-r < mousePos.y) {
//        FragColor = vec4(1.0,1.0,1.0,1.0);
    } else {
//        FragColor = vec4(0.0,0.1,0.1,1.0);
    }
    
    int iterations = 256;
    vec2 position = vec2(gl_FragCoord.x/screenSize.x-0.5,gl_FragCoord.y/screenSize.y-0.5); // gets the location of the current pixel in the intervals [0..1] [0..1]
    vec3 color = vec3(0.0,0.0,0.0); // initialize color to black
    
    vec2 z = position; // z.x is the real component z.y is the imaginary component
    
    
    // Rescale the position to the intervals [-2,1] [-1,1]
    z *= vec2(3.0,2.0);
    z -= vec2(2.0,1.0);
    
    
    z.x /= pow(1.003,time);
    z.y /= pow(1.003,time);
    
    z.x += setCenter.x;
    z.y -= setCenter.y;
    
    // SET TO ZERO TO STOP JULIA ANIMATION
//    float setCenterOffset = time/10000.0-15/1000.0;
    
    // MOVING JULIA
//    vec2 c = vec2(-0.7+setCenterOffset,0.4+setCenterOffset);
    
    // MANDEL
    vec2 c = z;

    
    float it = 0.0; // Keep track of what iteration we reached
    for (int i = 0;i < iterations; ++i) {
        // zn = zn-1 ^ 2 + c
        
        // (x + yi) ^ 2 = x ^ 2 - y ^ 2 + 2xyi
        
        // TRADITIONAL MANDEL OR JULIA
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y);
        z += c;
        
        // JULIA VARIATION
//        z = vec2(z.x * z.x * z.x - 3.0 * z.x * z.y * z.y, 3 * z.x * z.x * z.y - z.y * z.y * z.y);
//        z = vec2(z.x-1,z.y);
        
        if (dot(z,z) > 4.0) { // dot(z,z) == length(z) ^ 2 only faster to compute
            break;
        }
        
        it += 1.0;
    }
    
    if (it < float(iterations)) {
        color.x = sin(it / 3.0);
        color.y = cos(it / 6.0);
        color.z = cos(it / 12.0 + 3.14 / 4.0);
    }
//    color.x = it/float(iterations);
    
    FragColor = vec4(color,1.0);
}