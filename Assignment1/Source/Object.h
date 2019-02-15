#ifndef OBJECT_H
#define OBJECT_H

#include "Joint.h"
#include "BoundingBox.h"

class Object {

private:

	Joint * start, *end;
	
	float mass, length;
	Vector3 scale, rotation, center;

	Vector3 velocity;

	BoundingBox bb;

	bool affectByGravity;

	void update();

public:

	Object(Joint* = nullptr, Joint* = nullptr, float = 0, float = 2);
	Object(Vector3, Vector3, float = 0, bool = true);

	Joint* getStart();
	Joint* getEnd();
	float getMass();
	Vector3 getScale();
	Vector3 getRotation();
	Vector3 getCenter();
	Vector3 getMomentum();
	BoundingBox& getBoundingBox();
	bool isAffectByGravity();

	void constraint();
	void accelerate(Vector3, float);
	void applyImpulse(Vector3, float);
	void applyCircularImpulse(Vector3, float);

};

#endif