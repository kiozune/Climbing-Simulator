#include "MainScene.h"

//Renders Main Menu
void MainScene::renderMenu()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Using Shader without the shadow calculation

	glUseProgram(menuShader);
	//renders texts and main menu
	renderMenu2D(models[MAINMENU_QUAD], 11.0f, 14.0f, 11.0f, -0.8f, 1.25f);
	renderTextOnScreenMenu(models[JOINONLINE_QUAD], "JOIN ONLINE", Color(OnlineR, OnlineG, 1), onlineSize, 4, 20);
	renderTextOnScreenMenu(models[STARTLOCAL_QUAD], "START LOCAL", Color(start_LocalR, start_LocalG, 1), start_LocalSize, 4, 15);
	renderTextOnScreenMenu(models[JOINLOCAL_QUAD], "JOIN LOCAL", Color(localR, localG, 1), localSize, 4, 10);
	renderTextOnScreenMenu(models[EXIT], "EXIT", Color(exitR, exitG, 1), exitSize, 4, 5);

	glEnable(GL_DEPTH_TEST);
	modelStack.PushMatrix();
	modelStack.Translate(0.45f, -0.4f, -0.4f);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	modelStack.Rotate(rotateMap, 0, 1, 0);
	modelStack.Rotate(25, 1, 0, 0);
	renderMeshMenu(models[TEST_OBJ], false);
	modelStack.PopMatrix();
	glDisable(GL_DEPTH_TEST);
}