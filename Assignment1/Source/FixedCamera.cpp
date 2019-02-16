#include "FixedCamera.h"

#include "Utility.h"

void FixedCamera::update()
{
	Vector3 offset;
	offset.x = cos(rad(yaw)) * cos(rad(pitch));
	offset.y = sin(rad(pitch));
	offset.z = sin(rad(yaw)) * cos(rad(pitch));

	this->position = this->target - offset * radius;

	Vector3 f = target - this->position;
	if (f.IsZero()) return;

	this->front = f.Normalize();
	this->right = this->front.Cross(this->worldUp).Normalize();
	this->up = this->right.Cross(this->front).Normalize();
}

void FixedCamera::Init(const Vector3& target, const GLfloat radius, const GLfloat speed)
{
	this->pitch = this->yaw = 0;
	this->radius = radius;	
	this->turnSpeed = speed;
	this->worldUp = Vector3(0, 1, 0);
	this->setTarget(target);

	this->update();
}

void FixedCamera::changeYaw(float r, float dt)
{
	this->yaw += this->turnSpeed * r * dt;
	this->update();
}

void FixedCamera::changePitch(float r, float dt)
{
	this->pitch += this->turnSpeed * r * dt;

	if (pitch > 89)
		pitch = 89;
	else if (pitch < -89)
		pitch = -89;

	this->update();
}

void FixedCamera::moveTo(float dt)
{
	
}

void FixedCamera::setAuto(bool b) { this->automatic = b; }
bool FixedCamera::isAuto() { return this->automatic; }

void FixedCamera::setTarget(Vector3 target)
{
	this->target = target;
	this->update();
}