#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <vector>

#include "Vector3.h"
#include "Mtx44.h"
#include "CollisionResult.h"

class BoundingBox 
{

private: 

	std::vector<Vector3> raw;
	std::vector<Vector3> transformed;

	Vector3 P, X, Y, Z;
	float rawW, rawH, rawD;
	float W, H, D;

	Mtx44 rotation, scale, translation;

	BoundingBox* update();

public:

	BoundingBox();

	void setVertices(Vector3, Vector3);
	std::vector<Vector3> getVertices();
	
	BoundingBox* setRotation(float, float);
	BoundingBox* setScale(Vector3);
	BoundingBox* setTranslation(Vector3);

	CollisionResult getCollisionResultWith(BoundingBox&);

};

#endif