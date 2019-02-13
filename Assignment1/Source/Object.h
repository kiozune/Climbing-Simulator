#ifndef OBJECT_H
#define OBJECT_H

#include "Bone.h"

class Object {

private:

	Bone* bone;

public:

	Bone * getBone();
	void setBone(Bone*);

};

#endif