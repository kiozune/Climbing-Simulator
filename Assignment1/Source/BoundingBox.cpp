#include "BoundingBox.h"

#include "Utility.h"

BoundingBox* BoundingBox::update()
{
	for (int i = 0; i < raw.size(); ++i)
		transformed[i] = rotation * scale * translation * raw[i];

	return this;
}

BoundingBox::BoundingBox(std::vector<Vector3> v)
{
	this->raw = v;
	this->transformed = v;

	this->rotation.SetToIdentity();
	this->scale.SetToIdentity();
	this->translation.SetToIdentity();
}

BoundingBox* BoundingBox::setRotation(float x, float y, float z)
{
	Mtx44 roll; roll.SetToRotation(x, 1, 0, 0);
	Mtx44 yaw; yaw.SetToRotation(y, 0, 1, 0);
	Mtx44 pitch; pitch.SetToRotation(z, 0, 0, 1);

	this->rotation = roll * yaw * pitch;

	return this->update();
}

BoundingBox* BoundingBox::setScale(float x, float y, float z)
{
	this->scale.SetToScale(x, y, z);

	return this->update();
}

BoundingBox* BoundingBox::setTranslation(float x, float y, float z)
{
	this->translation.SetToTranslation(x, y, z);

	return this->update();
}

