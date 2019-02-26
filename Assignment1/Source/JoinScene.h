#ifndef JOIN_SCENE_H
#define JOIN_SCENE_H

#include "Scene.h"

class JoinScene : public Scene
{

private:

	float elapseTime = 0;
	float bounceTime; // for key press inputs

	std::string ip;

	Mesh* text;
	
	MS modelStack, viewStack, projectionStack;

	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

public:

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

};

#endif