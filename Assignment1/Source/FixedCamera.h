#ifndef FIXED_CAMERA_H
#define FIXED_CAMERA_H

#include "Camera.h"

class FixedCamera : public Camera
{

private:

	// to store standard values
	GLfloat pitch, yaw, radius;
	GLfloat moveSpeed, turnSpeed;

	bool automatic;

	virtual void update();

public:

	void Init(const Vector3&, const GLfloat, const GLfloat, const GLfloat);
	
	void zoomIn(float);
	void zoomOut(float);

	void changeYaw(float, float);
	GLfloat getYaw();

	void changePitch(float, float);
	GLfloat getPitch();

	void setAuto(bool);
	bool isAuto();

	virtual void setTarget(Vector3);

};

#endif