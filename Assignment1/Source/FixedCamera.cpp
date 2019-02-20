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

void FixedCamera::Init(const Vector3& target, const GLfloat radius, const GLfloat moveSpeed, const GLfloat turnSpeed)
{
	this->pitch = 0;
	this->yaw = 90;
	this->radius = radius;	
	this->moveSpeed = moveSpeed;
	this->turnSpeed = turnSpeed;
	this->worldUp = Vector3(0, 1, 0);
	this->target = target;

	this->update();
}

void FixedCamera::zoomIn(float dt) { this->radius -= moveSpeed * dt; }
void FixedCamera::zoomOut(float dt) { this->radius += moveSpeed * dt; }

void FixedCamera::changeYaw(float r, float dt)
{
	this->yaw += this->turnSpeed * r * dt;
	this->update();
}

GLfloat FixedCamera::getYaw() { return this->yaw; }

void FixedCamera::changePitch(float r, float dt)
{
	this->pitch += this->turnSpeed * r * dt;

	if (pitch > 89)
		pitch = 89;
	else if (pitch < -89)
		pitch = -89;

	this->update();
}

GLfloat FixedCamera::getPitch() { return this->pitch; }

void FixedCamera::setTarget(Vector3 v)
{
	Vector3 d = v - this->target;
	this->target += d * 0.1;
	this->update();
}