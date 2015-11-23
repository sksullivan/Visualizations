#import "../common.h"
#include "glm/ext.hpp"
#import <glm/gtc/matrix_transform.hpp>
#import "Geometry.h"
#import "../renderer.h"

class Card {
private:
    std::vector<Geometry*>* partsGeom;
    std::vector<RenderObject*>* partsObjs;
    static int colorIndex;
    int cardPartCount;
    
    std::vector<glm::vec3>* partRandVecs;
    glm::vec3 initialPosition;
    float initialRotation, explodeAtStep;
public:
    Card(glm::vec3 position, float rotation, float explodeAtStep);
    static glm::vec3 randExplosionVec();
    static glm::vec3 randPositionVec();
    void addGeometryToRenderer(Renderer* renderer);
    void updateTransformsForTimeStep(int elapsedSteps);
    void explode();
};