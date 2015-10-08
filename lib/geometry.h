#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "../common.h"
#include "glm/ext.hpp"

/********************************

Class:	Geometry

Purpose: Class for handling obj
		 model files.

********************************/
using namespace std;

class Geometry
{
public:
	bool loadData(std::string);
	void bufferDataToGPU();
	void releaseData();
	void printData();
	int getPolygonCount();
	void render();

	Geometry();
    
private:
	bool bLoaded;
	int iAttrBitField;
	int iNumFaces;
	
	vector<glm::vec3> vVertices;
	vector<glm::vec2> vTexCoords;
	vector<glm::vec3> vNormals;
	vector<glm::vec3> fVertices;

	GLuint vbo;
	GLuint vao;
};

#endif