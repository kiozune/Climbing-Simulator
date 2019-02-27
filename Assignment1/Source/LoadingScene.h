#ifndef LOADING_SCENE_H
#define LOADING_SCENE_H

#include "Scene.h"

class LoadingScene : public Scene
{

private:

	float elapseTime = 0;
	float bounceTime; // for key press inputs

	Mesh* text;

	MS modelStack, viewStack, projectionStack;

	int state;
	Scene *destination, *source;
	void (*action)(int&);

	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

public:

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void setDetails(void(*a)(int&), Scene*, Scene*);
	void renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

};

#endif