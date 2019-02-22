#include "block.h"

block::block()
{
	next = nullptr;
	previous = nullptr;

	position = Vector3(0, 0, 0);
}
/// default a quad mesh
void block::setMesh()
{
	meshes = new std::vector<Mesh*>;
	cubePos = new std::vector<Vector3>;
	universalSizing = new std::vector<Vector3>;
	Mesh* mesh = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), Position(1, 1, 1));
	meshes->push_back(mesh);
	cubePos->push_back(position);
	universalSizing->push_back(Vector3(1, 1, 1)); // default
}

void block::setMesh(float size)
{
	Mesh* mesh = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), size, size, size);
	meshes->push_back(mesh);
}

void block::setCubeVector()
{
	Vector3 newPos = { getRandomFloat(-0.5,0.5),getRandomFloat(-0.5,0.5),getRandomFloat(-0.5,0.5) };
	newPos += position;
	cubePos->push_back(newPos);
}

void block::setNext(block * val)
{
	next = val;
}

void block::setPrevious(block * val)
{
	previous = val;
}

void block::populateNode(int amount, float cube_Size)
{
	meshes = new std::vector<Mesh*>;
	cubePos = new std::vector<Vector3>;
	universalSizing = new std::vector<Vector3>;
	meshes->reserve(amount);
	cubePos->reserve(amount);
	for (int i = 0; i < amount; ++i)
	{
		setMesh(cube_Size); // populate meshes
		setCubeVector(); // populate their location
	}
	universalSizing->push_back(Vector3(cube_Size, cube_Size, cube_Size));
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

std::vector<Mesh*> * block::getMeshes()
{
	return meshes;
}

std::vector<Vector3> * block::getCubePos()
{
	return cubePos;
}

std::vector<Vector3>* block::getUniSizing()
{
	return universalSizing;
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
