#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <vector>

#include "Vector3.h"
#include "Mtx44.h"

class BoundingBox 
{

private: 

	std::vector<Vector3> raw;
	std::vector<Vector3> transformed;

	Mtx44 rotation, scale, translation;

	BoundingBox* update();

public:

	BoundingBox(std::vector<Vector3>);
	
	BoundingBox* setRotation(float, float, float);
	BoundingBox* setScale(float, float, float);
	BoundingBox* setTranslation(float, float, float);

};

#endif