#include "Camera.h"

#include "Utility.h"
#include "Mtx44.h"

void Camera::update() {
	front.x = cos(rad(yaw)) * cos(rad(pitch));
	front.y = sin(rad(pitch));
	front.z = sin(rad(yaw)) * cos(rad(pitch));
	front = front.Normalize();

	right = front.Cross(worldUp).Normalize();
	up = right.Cross(front).Normalize();
	forward = right.Cross(worldUp).Normalize();
}

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera::Camera() {}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera::~Camera() {}

/*

	Purpose : Initlise moveable camra
	Params: 
		pos : position of camera
		worldUp : direction vector world up
		startPitch
		startYaw
		moveSpeed
		turnSpeed

*/
void Camera::Init(const Vector3& pos, const Vector3& worldUp, const GLfloat startPitch, const GLfloat startYaw, const GLfloat moveSpeed, const GLfloat turnSpeed)
{
	this->position = pos;
	this->worldUp = worldUp;

	this->moveSpeed = moveSpeed;
	this->turnSpeed = turnSpeed;

	this->pitch = startPitch;
	this->yaw = startYaw;

	update();
}

/*

	Purpose : Initlise stationary camra
	Params:
		pos : position of camera
		worldUp : direction vector world up
		target : point that camera is facing

*/
void Camera::Init(const Vector3& pos, const Vector3& worldUp, const Vector3& target)
{
	this->position = pos;
	this->worldUp = this->up = worldUp;
	this->target = target;

	this->moveSpeed = 0;
	this->turnSpeed = 0;
}

// returns target based on type of character
Vector3 Camera::getTarget() {
	// if moveSpeed is set to 0 this camera is stationary
	return moveSpeed ? this->position + this->front : this->target;
}

Vector3 Camera::getForward() {
	return this->forward;
}

Vector3 Camera::getUp() {
	return this->up;
}

void Camera::setTarget(Vector3 target) {
	this->target = target;
}

void Camera::setPosition(Vector3 pos) {
	this->position = pos;
}

void Camera::move(const double dt) {
	GLfloat velocity = moveSpeed * dt;

	if (Application::IsKeyPressed('W'))
		position -= forward * velocity;

	if (Application::IsKeyPressed('S'))
		position += forward * velocity;

	if (Application::IsKeyPressed('A'))
		position -= right * velocity;

	if (Application::IsKeyPressed('D'))
		position += right * velocity;

	if (Application::IsKeyPressed(VK_SHIFT))
		position -= worldUp * velocity;

	if (Application::IsKeyPressed(VK_SPACE))
		position += worldUp * velocity;
}

void Camera::lookAround(GLfloat xPos, GLfloat yPos) {
	GLfloat xDiff = (xPos - prevX) / 10.0;
	GLfloat yDiff = (yPos - prevY) / 10.0;

	if (xDiff) yaw += xDiff;
	if (yDiff) pitch -= yDiff;

	// lock pitch
	if (pitch > 89)
		pitch = 89;
	else if (pitch < -89)
		pitch = -89;

	prevX = xPos, prevY = yPos;

	// update vectors
	update();
}