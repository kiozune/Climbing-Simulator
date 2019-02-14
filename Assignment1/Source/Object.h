#ifndef OBJECT_H
#define OBJECT_H

#include "Joint.h"

class Object {

private:

	Joint * start, *end;
	
	float length, mass;

	Vector3 rotation, center;

	void update();

public:

	Object(Joint* = nullptr, Joint* = nullptr, float = 0);

	Joint* getStart();
	Joint* getEnd();
	float getLength();
	Vector3 getRotation();
	Vector3 getCenter();

	void constraint();
	void accelerate(Vector3, float);
	void applyImpulse(Vector3 force, float dt);

};

#endif