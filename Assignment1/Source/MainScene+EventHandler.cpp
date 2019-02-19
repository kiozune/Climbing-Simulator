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
		camera.changePitch(-1, dt);
	if (Application::IsKeyPressed(VK_DOWN))
		camera.changePitch(1, dt);
	if (Application::IsKeyPressed(VK_LEFT))
		camera.changeYaw(1, dt);
	if (Application::IsKeyPressed(VK_RIGHT))
		camera.changeYaw(-1, dt);
}

void MainScene::joystickEvents(double& dt)
{
	if (!Application::isControllerPresent()) return;

	//swingX = analog[0], swingY = analog[1];
	//LT = analog[4], RT = analog[5];
	//camX = analog[2], camY = analog[3];

	if (Application::IsControllerPressed(GLFW_JOYSTICK_2))
		camera.zoomIn(dt);

	if (Application::IsControllerPressed(GLFW_JOYSTICK_3))
		camera.zoomOut(dt);
	
	// camera
	Vector3 rightJS = controller->getRightJoystick();
	camera.changeYaw(rightJS.x, dt);
	camera.changePitch(rightJS.y, dt);
}