#ifndef FIXED_CAMERA_H
#define FIXED_CAMERA_H

#include "Camera.h"

class FixedCamera : public Camera
{

private:

	// to store standard values
	GLfloat pitch, yaw, radius;
	GLfloat turnSpeed;

	bool automatic;

	virtual void update();

public:

	void Init(const Vector3&, const GLfloat, const GLfloat);

	void changeYaw(float, float);
	void changePitch(float, float);

	void moveTo(float);

	void setAuto(bool);
	bool isAuto();

	virtual void setTarget(Vector3 target);

};

#endif