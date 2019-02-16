#include "MovableCamera.h"

#include "Utility.h"

void MoveableCamera::update()
{
	front.x = cos(rad(yaw)) * cos(rad(pitch));
	front.y = sin(rad(pitch));
	front.z = sin(rad(yaw)) * cos(rad(pitch));

	if (front.IsZero()) return;

	front = front.Normalize();
	right = front.Cross(worldUp).Normalize();
	up = right.Cross(front).Normalize();
	target = position + front;
	forward = right.Cross(worldUp).Normalize();	
}

void MoveableCamera::setTarget(Vector3 target) {}

void MoveableCamera::move(const double dt) 
{
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

void MoveableCamera::lookAround(GLfloat xPos, GLfloat yPos) 
{
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
