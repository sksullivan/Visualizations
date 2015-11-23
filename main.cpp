#include "renderer.h"
#include "lib/Card.h"
#include <sstream>

std::vector<Card*> cards;

void updateObjectsForTimeStep(int elapsedSteps) {
    for (auto card : cards) {
        card->updateTransformsForTimeStep(elapsedSteps);
    }
}

int main () {
    srand(time(NULL));
	Renderer mainRenderer(600,400);
    
    // CUBE
//    Geometry* cubeGeom = new Geometry();
//    cubeGeom->loadData("models/cube_2.obj");
//    RenderObject* cObj = new RenderObject(cubeGeom);
//    mainRenderer.addRenderObject(cObj);
//    mainRenderer.setShaders("shaders/tunnel-vert.glsl","shaders/tunnel-frag.glsl");
    
    // TUNNEL
//    Geometry* tunnelGeom = new Geometry();
//    tunnelGeom->loadData("models/tunnel.obj");
//    RenderObject* tunnelObj = new RenderObject(tunnelGeom);
//    std::vector<float> colors(6012*3*3,1.0f);
//    for (int i = 0; i < colors.size()/3; i++) {
//        std::tuple<float,float,float> color = rainbowColorForTime(i);
//        colors[3*i] = std::get<0>(color);
//        colors[3*i+1] = std::get<1>(color);
//        colors[3*i+2] = std::get<2>(color);
//    }
//    tunnelGeom->setColorData(colors);
//    mainRenderer.addRenderObject(tunnelObj);
//    mainRenderer.setShaders("shaders/tunnel-vert.glsl","shaders/tunnel-frag.glsl");
    
//     CONCENTRIC
//    Geometry* cubeGeom = new Geometry();
//    cubeGeom->loadData("models/cube_2.obj");
//    RenderObject* cObj = new RenderObject(cubeGeom);
//    mainRenderer.addRenderObject(cObj);
//    mainRenderer.setShaders("shaders/concentric-vert.glsl","shaders/concentric-frag.glsl");
    
    // Floating Cubes
//    Geometry* cubeGeom = new Geometry();
//    cubeGeom->loadData("models/cube_2.obj");
//    for (int i = 0; i < 9; i++) {
//        RenderObject* cObj = new RenderObject(cubeGeom);
//        mainRenderer.addRenderObject(cObj);
//    }
//    mainRenderer.setShaders("shaders/tunnel-vert.glsl","shaders/tunnel-frag.glsl");

    // Splody cubes
//    Geometry* cubeGeom = new Geometry();
//    cubeGeom->loadData("models/cube_2.obj");
//    std::vector<float> colors(12*3*3,1.0f);
//    for (int i = 0; i < colors.size()/3; i++) {
//        float iterProg = i/(colors.size()/3.0f);
//        colors[3*i] = 1.0f*iterProg;
//        colors[3*i+1] = 1.5f*iterProg;
//        colors[3*i+2] = 0.0f*iterProg;
//    }
//    cubeGeom->setColorData(colors);
//    for (int i = 0; i < 9; i++) {
//        RenderObject* cObj = new RenderObject(cubeGeom);
//        mainRenderer.addRenderObject(cObj);
//    }
//    mainRenderer.setShaders("shaders/tunnel-vert.glsl","shaders/tunnel-frag.glsl");
    
    // Crystals
//    Geometry* cubeGeom = new Geometry();
//    cubeGeom->loadData("models/hex.obj");
//    std::vector<float> colors(11520*3*3,1.0f);
//    int faceGroupIndex = 0;
//    float randLuminosity = 0.5;
//    for (int i = 0; i < colors.size()/3; i++) {
//        faceGroupIndex++;
//        if (faceGroupIndex > 20) {
//            faceGroupIndex = 0;
//            randLuminosity = (float)(rand()) / (float)(RAND_MAX);
//        }
//        std::tuple<float,float,float> color = rainbowColorForTime(i);
//        colors[3*i] = std::get<0>(color)*randLuminosity;
//        colors[3*i+1] = std::get<1>(color)*randLuminosity;
//        colors[3*i+2] = std::get<2>(color)*randLuminosity;
//    }
//    cubeGeom->setColorData(colors);
//    RenderObject* cObj = new RenderObject(cubeGeom);
//    mainRenderer.addRenderObject(cObj);
//    mainRenderer.setShaders("shaders/tunnel-vert.glsl","shaders/tunnel-frag.glsl");
    
    // Mandelbrot
    Geometry* cubeGeom = new Geometry();
    cubeGeom->loadData("models/cube_2.obj");
    RenderObject* cObj = new RenderObject(cubeGeom);
    mainRenderer.addRenderObject(cObj);
    mainRenderer.setShaders("shaders/mandel-vert.glsl","shaders/mandel-frag.glsl");

    
    // Cards
//    cards = std::vector<Card*>();
//    for (int i = 0; i < 100; i++) {
//        glm::vec3 randStartVec = Card::randPositionVec()*10.0f;
//        randStartVec.z /= 20.0f;
//        randStartVec.z -= 10.0f;
//        randStartVec.y *= 10.0f;
//        randStartVec.y -= 30.0f;
//        randStartVec.x *= 10.0f;
//        Card* card = new Card(randStartVec,(float)rand()/RAND_MAX*3.14f,(float)rand()/RAND_MAX*60.0f*60.0f);
//        cards.push_back(card);
//        card->addGeometryToRenderer(&mainRenderer);
//    }
//    mainRenderer.addUpdateFunction(updateObjectsForTimeStep);
//    mainRenderer.setShaders("shaders/tunnel-vert.glsl","shaders/tunnel-frag.glsl");
    
    
    // Newton Fractal
//    Geometry* cubeGeom = new Geometry();
//    cubeGeom->loadData("models/cube_2.obj");
//    RenderObject* cObj = new RenderObject(cubeGeom);
//    mainRenderer.addRenderObject(cObj);
//    mainRenderer.setShaders("shaders/mandel-vert.glsl","shaders/newton-frag.glsl");
    
    // See Renderer::populateUniforms for more customization code.
    
    mainRenderer.begin();
	return 0;
}