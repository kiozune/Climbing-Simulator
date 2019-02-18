#ifndef COLLISION_RESULT_H
#define COLLISION_RESULT_H

#include "Vector3.h"

struct CollisionResult
{
	bool collided = false;
	Vector3 displacement;
};

#endif