#include "Card.h"
#define NUM_CARD_PARTS 16

int Card::colorIndex = 0;

glm::vec3 Card::randExplosionVec() {
    double comps[3];
    for (int compIndex = 0; compIndex < 3; compIndex++) {
        comps[compIndex] = (double)rand()/(RAND_MAX)*2-1;
        if (std::abs(comps[compIndex]) < 0.5) {
            if (comps[compIndex] > 0) {
                comps[compIndex] = 0.5;
            } else {
                comps[compIndex] = -0.5;
            }
        }
    }
    return glm::vec3(comps[0],comps[1],comps[2]);
}

glm::vec3 Card::randPositionVec() {
    double comps[3];
    for (int compIndex = 0; compIndex < 3; compIndex++) {
        comps[compIndex] = (double)rand()/(RAND_MAX)*2-1;
    }
    return glm::vec3(comps[0],comps[1],comps[2]);
}

Card::Card(glm::vec3 position, float rotation, float explodeAtStep) {
    initialPosition = position;
    initialRotation = rotation;
    this->explodeAtStep = explodeAtStep;
    partsGeom = new std::vector<Geometry*>();
    for (int i = 0; i < NUM_CARD_PARTS; i++) {
        std::ostringstream fileName;
        fileName << "models/cardparts/Plane.";
        if (i < 10) {
            fileName << "00";
        } else {
            fileName << "0";
        }
        fileName << i << ".obj";
        Geometry* cardpartGeom = new Geometry();
        cardpartGeom->loadData(fileName.str());
        partsGeom->push_back(cardpartGeom);
    }
    
    partsObjs = new std::vector<RenderObject*>();
    partRandVecs = new std::vector<glm::vec3>();
    for (auto cardpartGeom : *partsGeom) {
        std::vector<float> colors(1*3*3,1.0f);
        for (int i = 0; i < colors.size()/3; i++) {
            std::tuple<float,float,float> color = rainbowColorForTime(colorIndex*50);
            colors[3*i] = std::get<0>(color);
            colors[3*i+1] = std::get<1>(color);
            colors[3*i+2] = std::get<2>(color);
        }
        cardpartGeom->setColorData(colors);
        RenderObject* cardpartObj = new RenderObject(cardpartGeom);
        partsObjs->push_back(cardpartObj);
        glm::vec3 randVec = randExplosionVec();
        partRandVecs->push_back(randVec);
    }
    std::cout << colorIndex << std::endl;
    colorIndex++;
}

void Card::addGeometryToRenderer(Renderer* renderer) {
    for (auto cardpartObj : *partsObjs) {
        renderer->addRenderObject(cardpartObj);
    }
}

void Card::updateTransformsForTimeStep(int elapsedSteps) {
    glm::mat4 transform = glm::mat4();
    transform = glm::translate(transform, initialPosition);
//    transform = glm::rotate(transform, initialRotation, glm::vec3(0.0f,1.0f,0.0f));
    transform = glm::translate(transform, glm::vec3(0.0f,elapsedSteps/50.0f,0.0f));
    if ((int)floor(explodeAtStep) > elapsedSteps) {
//        transform = glm::rotate(transform, elapsedSteps/100.0f, glm::vec3(0.0f,1.0f,0.0f));
    }
//    transform = glm::translate(transform, glm::vec3(0.0f,0.0f,elapsedSteps/5.0f));
    
    for (int partIndex = 0; partIndex < NUM_CARD_PARTS; partIndex++) {
        if ((int)floor(explodeAtStep) < elapsedSteps) {
            transform = glm::translate(transform, (*partRandVecs)[partIndex]*(elapsedSteps-(int)explodeAtStep)/50.0f);
        }
        (*partsObjs)[partIndex]->transform = transform;
    }
}