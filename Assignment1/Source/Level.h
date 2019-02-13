#pragma once
#include "MeshBuilder.h"
#include <string>

class level
{
	enum TEMPLATES
	{
		ARETE,
		VERTICAL,
		EMPTY,
		CUBE,
		NUM_TEMPLATE
	};
public:
	level();
	void setMesh(int ran);
	void setMesh(std::string model);
	Mesh getMesh();

	void setUP(level *val);
	void setDOWN(level *val);
	void setLEFT(level *val);
	void setRIGHT(level *val);

	void setVector3(Vector3 pos);
	Vector3 getVector3();

	level * getUP();
	level * getDOWN();
	level * getLEFT();
	level * getRIGHT();
	level * getDirection();

	~level();
private:
	level *UP;
	level *DOWN;
	level *LEFT;
	level *RIGHT;
	Mesh* mesh;

	Vector3 position;
};