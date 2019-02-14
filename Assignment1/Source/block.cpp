#include "block.h"

block::block()
{
	next = nullptr;
	previous = nullptr;

	mesh = nullptr;

	position = Vector3(0, 0, 0);
}

void block::setMesh(int val)
{
	switch (val)
	{
	case CUBE:
		mesh = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1, 1, 1);
		break;
	default:
		mesh = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1, 1, 1);
		break;
	}
}

Mesh * block::getMesh()
{
	return mesh;
}

void block::setNext(block * val)
{
	next = val;
}

void block::setPrevious(block * val)
{
	previous = val;
}

block * block::getNext()
{
	return next;
}

block * block::getPrevious()
{
	return previous;
}

int block::getMaxTemplate()
{
	return NUM_TEMPLATE;
}

void block::setVector3(Vector3 pos)
{
	position = pos;
}

Vector3 block::getVector3()
{
	return position;
}

block::~block()
{
}
