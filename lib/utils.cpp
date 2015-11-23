#include "utils.h"
#include <algorithm>

char* loadShader (std::string filename) {
    std::ifstream in(filename);
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    char* ret = (char*)malloc(sizeof(char)*contents.length());
    strcpy(ret,contents.c_str());
    return ret;
}

std::tuple<float,float,float> rainbowColorForTime(int elapsedSteps) {
    int r = 0;
    int g = 0;
    int b = 0;
    int colorStep = elapsedSteps % (255*6);
    if (colorStep < 255) {
        r = 255;
        g = colorStep;
    } else if (colorStep < 255*2) {
        r = 255-(colorStep-255);
        g = 255;
    } else if (colorStep < 255*3) {
        g = 255;
        b = colorStep-255*2;
    } else if (colorStep < 255*4) {
        g = 255-(colorStep-255*3);
        b = 255;
    } else if (colorStep < 255*5) {
        b = 255;
        r = colorStep-255*4;
    } else {
        b = 255-(colorStep-255*5);
        r = 255;
    }
//    std::cout << r/255.0 << "," <<  g/255.0 << "," << b/255.0 << std::endl;
    return std::make_tuple(r/255.0,g/255.0,b/255.0);
}

float periodicOffset(int step, float mag) {
    return mag*std::max(sinf((float)step/30.0f)-0.3,0.0);
}