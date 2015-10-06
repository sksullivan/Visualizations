#include "../common.h"
#include "glm/ext.hpp"

/********************************

Class:	CObjModel

Purpose: Class for handling obj
		 model files.

********************************/
using namespace std;

class CObjModel
{
public:
	bool loadModel(std::string);
	void bufferDataToGPU();
	void releaseModel();
	void printData();
	int getPolygonCount();
	void render();

	CObjModel();
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