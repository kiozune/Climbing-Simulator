#pragma once
#include "MeshBuilder.h"
#include <string>

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
	void setMesh();
	void setMesh(int val);
	Mesh *getMesh();

	void setNext(block *val);
	void setPrevious(block *val);

	block *getNext();
	block *getPrevious();

	int getMaxTemplate();

	void setVector3(Vector3 pos);
	Vector3 getVector3();

	
	~block();
private:
	block *next;
	block *previous;

	Mesh* mesh;
	
	Vector3 position;
};