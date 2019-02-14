#include "BoundingBox.h"

#include "Utility.h"

BoundingBox* BoundingBox::update()
{
	for (int i = 0; i < raw.size(); ++i)
		transformed[i] = translation *  rotation * scale * raw[i];

	this->P = translation * Vector3();
	this->X = (rotation * Vector3(1.0, 0, 0)).Normalized();
	this->Y = (rotation * Vector3(0, 1.0, 0)).Normalized();
	this->Z = (rotation * Vector3(0, 0, 1.0)).Normalized();

	return this;
}

BoundingBox::BoundingBox()
{
	this->rotation.SetToIdentity();
	this->scale.SetToIdentity();
	this->translation.SetToIdentity();
}

void BoundingBox::setVertices(Vector3 min, Vector3 max)
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

	this->W = this->rawW = fabs(min.x - max.x) / 2.0;
	this->H = this->rawH = fabs(min.y - max.y) / 2.0;
	this->D = this->rawD = fabs(min.z - max.z) / 2.0;

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
	this->W = this->rawW * s.x;
	this->H = this->rawH * s.y;
	this->D = this->rawD * s.z;

	return this->update();
}

BoundingBox* BoundingBox::setTranslation(Vector3 t)
{
	this->translation.SetToTranslation(t.x, t.y, t.z);

	return this->update();
}

bool BoundingBox::didCollideWith(BoundingBox& b) 
{
	Vector3 T = b.P - P;

	return !(
		fabs(T.Dot(X)) > W + fabs(b.W * X.Dot(b.X)) + fabs(b.H * X.Dot(b.Y)) + fabs(b.D * X.Dot(b.Z)) ||
		fabs(T.Dot(Y)) > H + fabs(b.W * Y.Dot(b.X)) + fabs(b.H * Y.Dot(b.Y)) + fabs(b.D * Y.Dot(b.Z)) ||
		fabs(T.Dot(Z)) > D + fabs(b.W * Z.Dot(b.X)) + fabs(b.H * Z.Dot(b.Y)) + fabs(b.D * Z.Dot(b.Z)) ||

		fabs(T.Dot(b.X)) > fabs(W * X.Dot(b.X)) + fabs(H * Y.Dot(b.X)) + fabs(D * Z.Dot(b.X)) + b.W ||
		fabs(T.Dot(b.Y)) > fabs(W * X.Dot(b.Y)) + fabs(H * Y.Dot(b.Y)) + fabs(D * Z.Dot(b.Y)) + b.H ||
		fabs(T.Dot(b.Z)) > fabs(W * X.Dot(b.Z)) + fabs(H * Y.Dot(b.Z)) + fabs(D * Z.Dot(b.Z)) + b.D ||

		fabs(T.Dot(X.Cross(b.X))) > fabs(H * Z.Dot(b.X)) + fabs(D * Y.Dot(b.X)) + fabs(b.H * X.Dot(b.Z)) + fabs(b.D * X.Dot(b.Y)) ||
		fabs(T.Dot(X.Cross(b.Y))) > fabs(H * Z.Dot(b.Y)) + fabs(D * Y.Dot(b.Y)) + fabs(b.W * X.Dot(b.Z)) + fabs(b.D * X.Dot(b.X)) ||
		fabs(T.Dot(X.Cross(b.Z))) > fabs(H * Z.Dot(b.Z)) + fabs(D * Y.Dot(b.Z)) + fabs(b.W * X.Dot(b.Y)) + fabs(b.H * X.Dot(b.X)) ||

		fabs(T.Dot(Y.Cross(b.X))) > fabs(W * Z.Dot(b.X)) + fabs(D * X.Dot(b.X)) + fabs(b.H * Y.Dot(b.Z)) + fabs(b.D * Y.Dot(b.Y)) ||
		fabs(T.Dot(Y.Cross(b.Y))) > fabs(W * Z.Dot(b.Y)) + fabs(D * X.Dot(b.Y)) + fabs(b.W * Y.Dot(b.Z)) + fabs(b.D * Y.Dot(b.X)) ||
		fabs(T.Dot(Y.Cross(b.Z))) > fabs(W * Z.Dot(b.Z)) + fabs(D * X.Dot(b.Z)) + fabs(b.W * Y.Dot(b.Y)) + fabs(b.H * Y.Dot(b.X)) ||

		fabs(T.Dot(Z.Cross(b.X))) > fabs(W * Y.Dot(b.X)) + fabs(H * X.Dot(b.X)) + fabs(b.H * Z.Dot(b.Z)) + fabs(b.D * Z.Dot(b.Y)) ||
		fabs(T.Dot(Z.Cross(b.Y))) > fabs(W * Y.Dot(b.Y)) + fabs(H * X.Dot(b.Y)) + fabs(b.W * Z.Dot(b.Z)) + fabs(b.D * Z.Dot(b.X)) ||
		fabs(T.Dot(Z.Cross(b.Z))) > fabs(W * Y.Dot(b.Z)) + fabs(H * X.Dot(b.Z)) + fabs(b.W * Z.Dot(b.Y)) + fabs(b.H * Z.Dot(b.X))
	);
}
