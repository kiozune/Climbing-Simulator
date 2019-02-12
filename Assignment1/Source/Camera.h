#ifndef CAMERA_H
#define CAMERA_H

#include "GL/glew.h"
#include "Vector3.h"

#include "Application.h"

class Camera {

private:

	// to store standard values
	GLfloat pitch, yaw;
	GLfloat moveSpeed, turnSpeed;

	// previous mouse position
	GLfloat prevX, prevY;

	/*
		store different values of directions
		front - facing direciton
		forward - movement direction
	*/
	Vector3 front, right, up, forward;
	Vector3 worldUp;

	/*
		to be used for player camera
		where the camera is just facing one position
	*/
	Vector3 target;

	// update the vectors based on yaw and pitch
	void update();

public:

	// store position of camera
	Vector3 position;

	Camera();
	~Camera();

	// initialisation for moveable camera
	void Init(const Vector3& pos, const Vector3& worldUp, const GLfloat startPitch, const GLfloat startYaw, const GLfloat moveSpeed, const GLfloat turnSpeed);
	// initialisation for stationary camera
	void Init(const Vector3& pos, const Vector3& worldUp, const Vector3& target);

	// getters
	Vector3 getTarget();
	Vector3 getForward();
	Vector3 getUp();

	// setter
	void setTarget(Vector3 target);
	void setPosition(Vector3 pos);

	// move camera based on key events and delta time
	void move(const double dt);
	// move camera facing direction base on mouse position
	void lookAround(GLfloat xDiff, GLfloat yDiff);
};

#endif