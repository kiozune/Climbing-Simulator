#include "MainScene.h"

#include "DataTransferManager.h"

//Loading Screen Render
void MainScene::RenderPause()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 modelView, modelView_inverse_transpose;


	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); // Remember, matrix multiplication is the other way around

	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP
	Mtx44 mTop = modelStack.Top();
	Mtx44 vTop = viewStack.Top();

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);

	DataTransferManager* d_manager = DataTransferManager::getInstance();
	std::string ip = d_manager->getClient().getServerIp();

	renderTextOnScreen(models[TEXT], "PAUSED\n" + ip, Color(), 2, 4, 15);
	renderTextOnScreen(models[RESUME_GAME], "RESUME", Color(), current == RESUME_GAME ? 4 : 3, 2, 4);
	renderTextOnScreen(models[EXIT_GAME], "EXIT", Color(), current == EXIT_GAME ? 4 : 3, 2, 2);
}