#ifndef OBJECT_H
#define OBJECT_H

#include "Bone.h"

class Object {

private:

	Bone* bone;
	const float mass;

public:

	Object(float, Bone* = nullptr);

	Bone * getBone();

	void applyImpulse(Vector3 force, float dt);

};

#endif