#version 140

uniform vec2 screenSize;
uniform float time;
uniform vec3 rainbowColor1;
uniform vec3 rainbowColor2;

out vec4 FragColor;

void main(void) {
    float x = gl_FragCoord.x - float(screenSize.x)/1.0;
    float y = gl_FragCoord.y - float(screenSize.y)/1.0;
    float radius = x*x + y*y;
    if (mod(radius/((time+1000.0)/30.0),5.0) < 2.0) {
        FragColor = vec4(rainbowColor1, 1.0);
    } else {
        FragColor = vec4(rainbowColor2, 1.0);
    }
}
