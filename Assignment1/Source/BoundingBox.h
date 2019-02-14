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

	BoundingBox();

	void setVertces(Vector3, Vector3);
	std::vector<Vector3> getVertices();
	
	BoundingBox* setRotation(float, float);
	BoundingBox* setScale(float, float, float);
	BoundingBox* setTranslation(float, float, float);

};

#endif