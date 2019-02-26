#include "MainScene.h"

//Loading Screen Render
void MainScene::renderLoading()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Use Menu Shader
	glUseProgram(menuShader);
	//Render Loading Screen
	renderMenu2D(models[LOADING], 11.0f, 14.0f, 11.0f, -0.8f, 1.25f);

	if (tempTime < elapseTime);
	e_States = GAMEMODE;
}