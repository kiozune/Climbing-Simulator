#ifndef	BONE_H
#define BONE_H

#include "Joint.h"

class Bone 
{

private:

	Joint *start, *end;
	float length;

	Vector3 rotation, center;

	void update();

public:

	Bone(Joint* = nullptr, Joint* = nullptr);

	Joint* getStart();
	Joint* getEnd();
	float getLength();
	Vector3 getRotation();
	Vector3 getCenter();

	void constraint();
	void accelerate(Vector3, float);

};

#endif