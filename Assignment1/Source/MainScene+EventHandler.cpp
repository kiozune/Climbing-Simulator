#include "MainScene.h"

void MainScene::keyboardEvents(double& dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if (Application::IsKeyPressed('7'))
		lightingEnabled = false;
	if (Application::IsKeyPressed('8'))
		lightingEnabled = true;

	if (Application::IsKeyPressed('9'))
		dt /= 100;

	if (Application::IsKeyPressed(VK_UP))
		cameras[0].changePitch(-1, dt);
	if (Application::IsKeyPressed(VK_DOWN))
		cameras[0].changePitch(1, dt);
	if (Application::IsKeyPressed(VK_LEFT))
		cameras[0].changeYaw(1, dt);
	if (Application::IsKeyPressed(VK_RIGHT))
		cameras[0].changeYaw(-1, dt);
}

void MainScene::joystickEvents(double& dt, int i)
{
	if (!controller->isPresent()) return;

	//swingX = analog[0], swingY = analog[1];
	//LT = analog[4], RT = analog[5];
	//camX = analog[2], camY = analog[3];

	if (Application::IsControllerPressed(GLFW_JOYSTICK_1 + i, 1))
		cameras[i].zoomIn(dt);

	if (Application::IsControllerPressed(GLFW_JOYSTICK_1 + i, 2))
		cameras[i].zoomOut(dt);
	
	// camera
	Vector3 rightJS = controller->getRightJoystick();
	cameras[i].changeYaw(rightJS.x, dt);
	cameras[i].changePitch(rightJS.y, dt);
}