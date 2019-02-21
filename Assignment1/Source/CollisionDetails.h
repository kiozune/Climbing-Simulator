#ifndef COLLISION_DETAILS_H
#define COLLISION_DETAILS_H

#include "CollisionResult.h"
#include "Object.h"

struct CollisionDetails 
{
	CollisionResult result;
	Object* object = nullptr;
};

#endif