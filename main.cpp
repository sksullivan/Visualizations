#include "renderer.h"

int main () {
	Renderer mainRenderer(600,400);
    
    Geometry* tunnelGeom = new Geometry();
    tunnelGeom->loadData("models/tunnel.obj");
    RenderObject* tunnelObj = new RenderObject(tunnelGeom);
    mainRenderer.addRenderObject(tunnelObj);
//    mainRenderer.setShaders("shaders/concentric-vert.glsl","shaders/concentric-frag.glsl");
//    mainRenderer.setShaders("shaders/tunnel-vert.glsl","shaders/tunnel-frag.glsl");
    
    // See Renderer::populateUniforms for more customization code.
    
    mainRenderer.begin();
	return 0;
}