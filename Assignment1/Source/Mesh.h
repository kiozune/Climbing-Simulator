#ifndef MESH_H
#define MESH_H

#include "GL\glew.h"
#include <string>

#include "Material.h"
#include "vertex.h"
#include <vector>

enum DRAW_MODE
{
	DRAW_TRIANGLES, //default mode
	DRAW_TRIANGLE_STRIP,
	DRAW_TRIANGLE_FAN,
	DRAW_LINES,
	DRAW_MODE_LAST,
};

/******************************************************************************/
/*!
		Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/
class Mesh
{

	// stores id to buffers
	unsigned vertexBuffer;
	unsigned colorBuffer;
	unsigned indexBuffer;
	unsigned textureBuffer;

	unsigned indexSize;

	// name of model
	const std::string name;

	DRAW_MODE mode;


	void calculateBB(std::vector<Vertex>);

public:
	
	Material material;

	Mesh();
	Mesh(const std::string &meshName);
	~Mesh();

	// initialise mesh with data
	void init(std::vector<Vertex> vertex_buffer_data, std::vector<GLuint> index_buffer_data, DRAW_MODE mode = DRAW_TRIANGLES, const bool rigidBody = false);
	void render();
	void render(unsigned offset, unsigned count);

	void applyTexture(const char* path);
	bool isTextured();
	unsigned getTextureID();
};

#endif