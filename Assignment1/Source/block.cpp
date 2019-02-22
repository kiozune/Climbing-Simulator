#include "block.h"

block::block()
{
	next = nullptr;
	previous = nullptr;

	position = Vector3(0, 0, 0);
}

void block::setCubeVector(float offset)
{
	Vector3 newPos = { getRandomFloat(-offset/2,offset/2),getRandomFloat(-offset/2,offset/2),getRandomFloat(-offset/2,offset/2) };
	newPos += position;
	cubeVectors->push_back(newPos);
}

void block::setNext(block * val)
{
	next = val;
}

void block::setPrevious(block * val)
{
	previous = val;
}

void block::populateNode(int amount, float offset)
{
	cubeVectors = new std::vector<Vector3>;
	cubeVectors->reserve(amount);
	for (int i = 0; i < amount; ++i)
	{
		setCubeVector(offset); // populate their location
	}

}

block * block::getNext()
{
	return next;
}

block * block::getPrevious()
{
	return previous;
}

void block::setVector3(Vector3 pos)
{
	position = pos;
}

std::vector<Vector3> * block::getCubeVectors()
{
	return cubeVectors;
}

Vector3 block::getVector3()
{
	return position;
}

block::~block()
{
}

/// Generate a number from 0.0 to an N float value
float block::getRandomFloat(float LO, float HI)
{
	return LO + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX/(HI-LO)));
}

int block::getRandomInt(int LO, int HI)
{
	return LO + (rand()) / (RAND_MAX/(HI-LO));
}
