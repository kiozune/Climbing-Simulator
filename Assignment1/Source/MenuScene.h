#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "Scene.h"

class MenuScene : public Scene
{
	
private:

	float elapseTime = 0;
	float bounceTime; // for key press inputs
	
					  /*Enum for gameStates Add Extra scenes below Exit_Game
	If Add Above EXIT_GAME modify scene->getSceneEnum() == 2 to + 1 in application.cpp*/
	enum OPTION
	{
		CREATE_LOBBY,
		JOIN_LOBBY,
		EXIT_GAME,
		COUNT,
	};
	OPTION current;

	ControllerManager* controller = ControllerManager::getInstance();

	Mesh* background;
	Mesh* models[COUNT];

	MS modelStack, viewStack, projectionStack;

	//Textures for Texts.
	unsigned t_opaque;
	unsigned t_alpha;
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

public:

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void renderMenu2D(Mesh* model, float sizex, float sizey, float sizez, float x, float y);

};

#endif