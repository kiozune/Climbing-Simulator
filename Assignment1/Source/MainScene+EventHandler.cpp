#include "AllScenes.h"
#include "SceneManager.h"

#include "DataTransferManager.h"

void MainScene::keyboardEvents(double& dt)
{
	Vector3 leftJS;
	if (controller->isPresent(0))
	{
		controller->getInput(0);
		leftJS = controller->getLeftJoystick();
	}

	if (isPaused)
	{
		if (elapseTime < bounceTime)
			return;

		if (Application::IsKeyPressed(VK_UP) || leftJS.y > 0.5)
		{
			current = (OPTION)(current - 1);
			if (current < 0) current = (OPTION)(OPTION::COUNT - 1);
			bounceTime = elapseTime + 0.2f;
		}

		if (Application::IsKeyPressed(VK_DOWN) || leftJS.y < -0.5)
		{
			current = (OPTION)((current + 1) % OPTION::COUNT);
			bounceTime = elapseTime + 0.2f;
		}

		models[RESUME_GAME]->setTexture(t_alpha);
		models[EXIT_GAME]->setTexture(t_alpha);

		models[current]->setTexture(t_opaque);

		if (Application::IsKeyPressed(VK_RETURN) || Application::IsControllerPressed(0,1))
		{
			switch (current)
			{
			case RESUME_GAME:
			{
				isPaused = false;
				break;
			}
			default:
				LoadingScene * destination = new LoadingScene;
				destination->setDetails([](int& i) {
					MultiplayerManager* m_manager = MultiplayerManager::getInstance();
					m_manager->end();
					DataTransferManager* d_manager = DataTransferManager::getInstance();
					d_manager->getClient().exit();
					i = 1;
				}, new MenuScene, this);

				SceneManager* s_manager = SceneManager::getInstance();
				s_manager->setNext(destination);
				break;
			}
			bounceTime = elapseTime + 0.2f;
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
			cameras[0].changePitch(-1, (float)dt);
		if (Application::IsKeyPressed(VK_DOWN))
			cameras[0].changePitch(1, (float)dt);
		if (Application::IsKeyPressed(VK_LEFT))
			cameras[0].changeYaw(1, (float)dt);
		if (Application::IsKeyPressed(VK_RIGHT))
			cameras[0].changeYaw(-1, (float)dt);
	}

	if (Application::IsKeyPressed(VK_ESCAPE) && elapseTime > bounceTime || Application::IsControllerPressed(0,9) && elapseTime > bounceTime)
	{
		isPaused = !isPaused;
		bounceTime = elapseTime + 0.2f;
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
		cam.zoomIn((float)dt);

	if (Application::IsControllerPressed(joy, 2))
		cam.zoomOut((float)dt);
	
	// camera
	Vector3 rightJS = controller->getRightJoystick();
	cam.changeYaw(rightJS.x, (float)dt);
	cam.changePitch(rightJS.y, (float)dt);
}