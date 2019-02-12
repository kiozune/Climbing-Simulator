#include "MeshBuilder.h"
#include <GL\glew.h>

#include "LoadOBJ.h"
#include "Utility.h"

void MeshBuilder::add(std::vector<Vertex> &data, const float x, const float y, const float z, Color color, const float u, const float v) {
	Vector3 normal = Vector3(x, y, z).Normalize();
	Vertex vert;

	vert.pos.Set(x, y, z);
	vert.normal.Set(normal.x, normal.y, normal.z);
	vert.color.Set(color.r, color.g, color.b);
	vert.texCoord.Set(u, v);
	data.push_back(vert);
}

Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, Position len)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	
	len = Position(len.x / 2 , len.y / 2, len.z / 2);
	
	v.pos.Set(-len.x, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(len.x, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	
	v.pos.Set(0, -len.y, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, len.y, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
		
	v.pos.Set(0, 0, -len.z);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, 0, len.z);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data = {
		0, 1, 2,
		3, 4, 5
	};

	Mesh *mesh = new Mesh(meshName);
	mesh->init(vertex_buffer_data, index_buffer_data, DRAW_LINES);
	
	return mesh;
}

Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ)
{
	// An array of 3 vectors which represents 3 vertices
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color;
	v.normal.Set(-0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, -0.5f); v.color = color;
	v.normal.Set(0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, -0.5f); v.color = color;
	v.normal.Set(0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, -0.5f); v.color = color;
	v.normal.Set(-0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.5f); v.color = color;
	v.normal.Set(-0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f); v.color = color;
	v.normal.Set(0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.5f); v.color = color;
	v.normal.Set(0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.5f); v.color = color;
	v.normal.Set(-0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(0);

	Mesh *mesh = new Mesh(meshName);
	mesh->init(vertex_buffer_data, index_buffer_data);

	return mesh;
}
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, Position size) 
{
	std::vector<Vertex> vertex_buffer_data;

	float width = size.x / 2.0;
	float height = size.y / 2.0;

	add(vertex_buffer_data, 0, -height,  width, color, 0, 0);
	add(vertex_buffer_data, 0, -height, -width, color, 2, 0);
	add(vertex_buffer_data, 0,  height, -width, color, 2, 2);
	add(vertex_buffer_data, 0,  height,  width, color, 0, 2);

	std::vector<GLuint> index_buffer_data = {
		0, 1, 2,
		0, 2, 3	
	};

	Mesh *mesh = new Mesh(meshName);
	mesh->init(vertex_buffer_data, index_buffer_data);

	return mesh;
}

Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, GLfloat radius, int slices)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float angle = 360.0 / (float)slices;

	for (int stack = 0; stack < slices; ++stack) 
	{
		float phi = rad(stack * angle);
		float phi2 = rad((stack + 1) * angle);
		for (int s = 0; s <= slices; ++s) 
		{
			float theta = rad(s * angle);
			add(vertex_buffer_data, radius * cos(phi) * cos(theta),  radius * sin(phi),  radius * cos(phi) * sin(theta),  color);
			index_buffer_data.push_back(index_buffer_data.size());

			add(vertex_buffer_data, radius * cos(phi2) * cos(theta), radius * sin(phi2), radius * cos(phi2) * sin(theta), color);
			index_buffer_data.push_back(index_buffer_data.size());
		}
	}

	Mesh *mesh = new Mesh(meshName);
	mesh->init(vertex_buffer_data, index_buffer_data, DRAW_TRIANGLE_STRIP);

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const bool rigidBody)
{
	std::string obj = "Object//" + meshName + ".obj";
	// std::string tex = "Image//" + meshName + ".tga";

	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;

	bool success = LoadOBJ(obj.c_str(), vertices, uvs, normals);

	if (!success) return NULL;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh *mesh = new Mesh(meshName);
	mesh->init(vertex_buffer_data, index_buffer_data, DRAW_TRIANGLES, rigidBody);
	// mesh->applyTexture(tex.c_str());

	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol) 
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for (unsigned i = 0; i < numRow; ++i)
	{
		for (unsigned j = 0; j < numCol; ++j)
		{
			float u1 = j * width;
			float v1 = 1.f - height - i * height;
			v.pos.Set(-0.5, -0.5, 0);
			v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5, -0.5, 0);
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5, 0.5, 0);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5, 0.5, 0);
			v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 3);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			offset += 4;
		}
	}

	Mesh *mesh = new Mesh(meshName);
	mesh->init(vertex_buffer_data, index_buffer_data);

	return mesh;
}