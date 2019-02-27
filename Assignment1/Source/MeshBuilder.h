#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{

private:

	// sets position, normal and colour of vertex and push back to data
	static void add(std::vector<Vertex> &data, const float x, const float y, const float z, Color color, const float u = 0, const float v = 0);

public:

	static Mesh* GenerateAxes(const std::string &meshName, Position len);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, Position size);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, GLfloat radius, int slices);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateScreen(const std::string &meshName, Color color, float length);
	// generate from an obj file
	static Mesh* GenerateOBJ(const std::string &meshName);
	static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);
};

#endif