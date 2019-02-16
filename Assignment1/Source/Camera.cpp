#include "Camera.h"

void Camera::Init(const Vector3& position)
{
	this->position = position;
	this->worldUp = Vector3(0, 1, 0);
	this->front = Vector3(1, 0, 0);

	this->update();
}

Vector3 Camera::getPosition() { return this->position; }
void Camera::setPosition(Vector3 position) { this->position = position; }

Vector3 Camera::getTarget() { return this->target; }

Vector3 Camera::getUp() { return this->up; }
