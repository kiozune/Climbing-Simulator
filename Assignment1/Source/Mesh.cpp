
#include "Mesh.h"
#include "GL\glew.h"

#include "LoadTGA.h"

void Mesh::calculateBB(std::vector<Vertex> data) {
	double maxX, minX, maxZ, minZ;
	maxX = minX = maxZ = minZ = -1;
	for (Vertex& v : data) {
		Position position = v.position;
		if (position.x < minX || minX == -1) minX = position.x;
		else if (position.x > maxX || maxX == -1) maxX = position.x;

		if (position.z < minZ || minZ == -1) minZ = position.z;
		else if (position.z > maxZ || minZ == -1) maxZ = position.z;
	}

	std::vector<Vector3> vertices = {
		Vector3(minX, 0, minZ),
		Vector3(minX, 0, maxZ),
		Vector3(maxX, 0, minZ),
		Vector3(maxX, 0, maxZ)
	};
}

Mesh::Mesh() {}

/******************************************************************************/
/*!
\brief
Default constructor - generate VBO/IBO here

\param meshName - name of mesh
*/
/******************************************************************************/
Mesh::Mesh(const std::string &meshName)
	: name(meshName)
	, mode(DRAW_TRIANGLES)
{
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &colorBuffer);
	glGenBuffers(1, &indexBuffer);
	textureBuffer = 0;

	for (int i = 0; i < max_Texture; ++i)
	{
		texArray[i] = 0;
	}
}

/******************************************************************************/
/*!
\brief
Destructor - delete VBO/IBO here
*/
/***********************ahh*******************************************************/
Mesh::~Mesh() {
	if (textureBuffer)
		glDeleteTextures(1, &textureBuffer);

	for (int i = 0; i < max_Texture; ++i)
	{
		glDeleteTextures(1, &texArray[i]);
	}
}

/*
	Purpose : Initlise mesh
	Params:
		vertex_buffer_data
		index_buffer_data
		mode : drawing mode

*/

unsigned Mesh::getShadowTexture(GLuint texture)
{
	this->textureBuffer = texture;

	return this->textureBuffer;
}

void Mesh::init(std::vector<Vertex> vertex_buffer_data, std::vector<GLuint> index_buffer_data, DRAW_MODE mode, const bool rigidBody)
{
	this->mode = mode;

	// set vertices in buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	this->indexSize = index_buffer_data.size();

	// set indices in buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	if (rigidBody) {
		calculateBB(vertex_buffer_data);
	}
}

/******************************************************************************/
/*!
\brief
OpenGL render code
*/
/******************************************************************************/
void Mesh::render()
{
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	
	if (textureBuffer) {
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	switch (mode)
	{
	case DRAW_TRIANGLES:
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
		break;
	case DRAW_TRIANGLE_STRIP:
		glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, 0);
		break;
	case DRAW_TRIANGLE_FAN:
		glDrawElements(GL_TRIANGLE_FAN, indexSize, GL_UNSIGNED_INT, 0);
		break;
	case DRAW_LINES:
		glDrawElements(GL_LINES, indexSize, GL_UNSIGNED_INT, 0);
		break;
	default:
		break;
	}

	if (textureBuffer) glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void Mesh::render(unsigned offset, unsigned count) {
	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));

	if (textureBuffer) {
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	switch (mode)
	{
	case DRAW_TRIANGLE_STRIP:
		glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
		break;
	case DRAW_LINES:
		glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
		break;
	default:
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
		break;
	}
	
	if (textureBuffer) glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void Mesh::applyTexture(const char* path) {
	this->textureBuffer = LoadTGA(path);
}

bool Mesh::isTextured() {
	return this->textureBuffer;
}

void Mesh::setTexture(unsigned int textureID)
{
	this->textureBuffer = textureID;
}

unsigned Mesh::getTextureID() {
	return this->textureBuffer;
}