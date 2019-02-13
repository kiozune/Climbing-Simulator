#include "Level.h"

level::level()
{
	UP = nullptr;
	DOWN = nullptr;
	LEFT = nullptr;
	RIGHT = nullptr;
}

void level::setMesh(int ran)
{
	switch (ran)
	{
	case ARETE:
		mesh = MeshBuilder::GenerateOBJ("arete");
		break;
	case VERTICAL:
		mesh = MeshBuilder::GenerateOBJ("vertical");
		break;
	case EMPTY:
		mesh = MeshBuilder::GenerateOBJ("empty");
		break;
	case CUBE:
		mesh = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 5, 5, 5);
	default:
		break;
	}
}

void level::setMesh(std::string model)
{
	mesh = MeshBuilder::GenerateOBJ(model);
}



Mesh level::getMesh()
{
	return* mesh;
}

void level::setUP(level *val)
{
	UP = val;
}

void level::setDOWN(level * val)
{
	DOWN = val;
}

void level::setLEFT(level * val)
{
	LEFT = val;
}

void level::setRIGHT(level * val)
{
	RIGHT = val;
}

void level::setVector3(Vector3 pos)
{
	position = pos;
}

level * level::getUP()
{
	return UP;
}

level * level::getDOWN()
{
	return DOWN;
}

level * level::getLEFT()
{
	return LEFT;
}

level * level::getRIGHT()
{
	return RIGHT;
}

level * level::getDirection()
{
	if (UP)
		return UP;
	else if (DOWN)
		return DOWN;
	else if (LEFT)
		return LEFT;
	else if (RIGHT)
		return RIGHT;
}

Vector3 level::getVector3()
{
	return position;
}

level::~level()
{
}
