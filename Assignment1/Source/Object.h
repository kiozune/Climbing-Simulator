#ifndef OBJECT_H
#define OBJECT_H

#include "Bone.h"

class Object {

private:

	Bone* bone;

public:

	Bone * getBone();
	void setBone(Bone*);

	void applyImpulse(Vector3 force, float dt);

};

#endif