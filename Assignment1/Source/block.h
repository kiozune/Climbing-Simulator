#pragma once
#include "MeshBuilder.h"
#include <string>
#include <vector>

class block
{
public:
	block();
	void setCubeVector(float offset);

	void setNext(block *val);
	void setPrevious(block *val);

	void populateNode(int amount, float offset);

	block *getNext();
	block *getPrevious();

	void setVector3(Vector3 pos);
	std::vector<Vector3> *getCubeVectors();
	Vector3 getVector3();

	~block();
private:
	block *next;
	block *previous;

	std::vector<Vector3> *cubeVectors;

	Vector3 position;

	float getRandomFloat(float LO, float HI);
	int getRandomInt(int LO, int HI);
};

