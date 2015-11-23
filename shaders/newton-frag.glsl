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
    
    z.x += setCenter.x*time*time/10.0;
    z.y -= setCenter.y*time*time/10.0;
    
    vec2 c = z;
    
    float it = 0.0; // Keep track of what iteration we reached
    for (int i = 0;i < iterations; ++i) {
        
        // z_n = z_(n-1) ^ 3 - 1
        // z^2 = (x + yi)^2 = x^2 - y^2 + 2xyi
        // z^3 = (x + yi)^3 = x^3 - 3xy^2 + 3yix^2 -iy^3
        // 3 -> z.x * z.x * z.x - 3.0 * z.x * z.y * z.y + 3 * z.x * z.x * z.y - z.y * z.y * z.y;
        // 2 -> z.x * z.x - z.y * z.y, 2.0 * z.x * z.y
        
        float a = 1.0;
        float p = z.x * z.x * z.x - 3.0 * z.x * z.y * z.y + 3 * z.x * z.x * z.y - z.y * z.y * z.y;
        p = p - 1.0;
        float pprime = 3.0 * (z.x * z.x - z.y * z.y, 2.0 * z.x * z.y);
        
        z = z - a*(p/pprime);
        
        z += c;
        
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