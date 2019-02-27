#ifndef OBJECT_H
#define OBJECT_H

#include "vertex.h"

#include "Joint.h"
#include "BoundingBox.h"

class Object {

private:

	Joint * start, *end;
	
	float mass, length, yaw, pitch;
	Vector3 scale, rotation, center;
	Color color;

	Vector3 velocity;

	BoundingBox bb;

	bool clippingEnabled, affectByGravity;

	void update();

public:

	Object(Joint* = nullptr, Joint* = nullptr, float = 0, float = 2);
	Object(Vector3, Vector3, float = 0, bool = true);

	Joint* getStart();
	Joint* getEnd();
	float getMass();
	Vector3 getScale();
	Vector3 getWorldScale();

	Vector3 getRotation();
	void setRotation(Vector3);

	Vector3 getCenter();
	Vector3 getMomentum();
	BoundingBox& getBoundingBox();
	bool isAffectByGravity();

	bool isClippingEnabled();
	void setClipping(bool);

	Color getColour();
	void setColour(Color);

	void constraint();
	void accelerate(Vector3, float);
	void applyImpulse(Vector3, float);

};

#endif