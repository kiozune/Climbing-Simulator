#pragma once
#include "MeshBuilder.h"
#include <string>
#include <vector>

//struct Obstacle
//{
//	Mesh* mesh;
//	Vector3 pos;
//	Obstacle(Mesh* cMesh, Vector3 cPos) : mesh(cMesh), pos(cPos) {} // Constructor
//};

enum BLOCK_TEMPLATE
{
	STARTING,
	CUBE,
	MONKEY_BAR,
	FINISH,
	NUM_TEMPLATE
};

class block
{
public:
	block();
	void setCubeVector();
	void setMesh();
	void setMesh(float size);

	void setNext(block *val);
	void setPrevious(block *val);

	void populateNode(int amount, float cube_Size);

	block *getNext();
	block *getPrevious();

	void setVector3(Vector3 pos);
	std::vector<Mesh*> *getMeshes();
	std::vector<Vector3> *getCubePos();
	std::vector<Vector3> *getUniSizing();
	Vector3 getVector3();

	~block();
private:
	block *next;
	block *previous;

	std::vector<Mesh*> *meshes;
	std::vector<Vector3> *cubePos;
	std::vector<Vector3> *universalSizing;

	Vector3 position;

	float getRandomFloat(float LO, float HI);
};

