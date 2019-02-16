#ifndef MOVEABLE_CAMERA_H
#define MOVEABLE_CAMERA_H

#include "Camera.h"

class MoveableCamera : public Camera
{

private:

	// previous mouse position
	GLfloat prevX, prevY;

	// to store standard values
	GLfloat pitch, yaw;
	GLfloat moveSpeed, turnSpeed;

	Vector3 forward;

	virtual void update();
	virtual void setTarget(Vector3 target);

public:

	// move camera based on key events and delta time
	void move(const double dt);

	// move camera facing direction base on mouse position
	void lookAround(GLfloat xDiff, GLfloat yDiff);


};

#endif