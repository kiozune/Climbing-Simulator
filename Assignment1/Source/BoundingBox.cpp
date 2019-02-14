#include "BoundingBox.h"

#include "Utility.h"

BoundingBox* BoundingBox::update()
{
	for (int i = 0; i < raw.size(); ++i)
		transformed[i] = translation *  rotation * scale * raw[i];

	return this;
}

BoundingBox::BoundingBox()
{
	this->rotation.SetToIdentity();
	this->scale.SetToIdentity();
	this->translation.SetToIdentity();
}

void BoundingBox::setVertces(Vector3 min, Vector3 max)
{
	for (int x = 0; x < 2; ++x)
	{
		float xVal = x % 2 ? min.x : max.x;
		for (int y = 0; y < 2; ++y)
		{
			float yVal = y % 2 ? min.y : max.y;
			for (int z = 0; z < 2; ++z)
			{
				float zVal = z % 2 ? min.z : max.z;
				this->raw.push_back(Vector3(xVal, yVal, zVal));
				this->transformed.push_back(Vector3(xVal, yVal, zVal));
			}
		}
	}
	this->update();
}

std::vector<Vector3> BoundingBox::getVertices() { return this->transformed; }

BoundingBox* BoundingBox::setRotation(float y, float z)
{
	Mtx44 yaw; yaw.SetToRotation(deg(y), 0, 1, 0);
	Mtx44 pitch; pitch.SetToRotation(deg(z), 0, 0, 1);

	this->rotation = yaw * pitch;

	return this->update();
}

BoundingBox* BoundingBox::setScale(Vector3 s)
{
	this->scale.SetToScale(s.x, s.y, s.z);

	return this->update();
}

BoundingBox* BoundingBox::setTranslation(Vector3 t)
{
	this->translation.SetToTranslation(t.x, t.y, t.z);

	return this->update();
}

bool BoundingBox::didCollideWith(BoundingBox& bb) 
{
	return false;
}
