#include "MainScene.h"

void MainScene::keyboardEvents(double& dt)
{
	if (e_States == JOIN_LOBBY)
	{
		char possible[11] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };

		if (elapseTime < bounceTime)
			return;

		for (char& c : possible)
		{
			if (Application::IsKeyPressed(c))
			{
				ip += c;
				bounceTime = elapseTime + 0.3;
				break;
			}
		}

		if (Application::IsKeyPressed(VK_BACK))
		{
			if (ip.size()) ip.pop_back();
			bounceTime = elapseTime + 0.3;
		}
	}
	else
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
}

void MainScene::joystickEvents(double& dt, int joy)
{
	if (!controller->isPresent()) return;
	FixedCamera& cam = cameras[joy + 1];

	//swingX = analog[0], swingY = analog[1];
	//LT = analog[4], RT = analog[5];
	//camX = analog[2], camY = analog[3];

	if (Application::IsControllerPressed(joy, 1))
		cam.zoomIn(dt);

	if (Application::IsControllerPressed(joy, 2))
		cam.zoomOut(dt);
	
	// camera
	Vector3 rightJS = controller->getRightJoystick();
	cam.changeYaw(rightJS.x, dt);
	cam.changePitch(rightJS.y, dt);
}