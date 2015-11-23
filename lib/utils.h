#import "../common.h"
#include <tuple>

char* loadShader(std::string);
std::tuple<float,float,float> rainbowColorForTime(int elapsedSteps);
float periodicOffset(int step, float mag);