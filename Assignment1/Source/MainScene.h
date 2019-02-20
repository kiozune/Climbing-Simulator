#ifndef MAINSCENE_H
#define MAINSCENE_H

#define LSPEED 10.0f

#include "Scene.h"
#include "..\FrameBufferObject.h"

#define LIGHT_COUNT 1

class MainScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		TEXT,
		LIGHT,
		SKY_BOX,
		TEST_OBJ,
		TEST_CUBE,
		SHADOW_QUAD,


		NUM_GEOMETRY,
	};

	enum e_Passes
	{
		FIRST_PASS,
		SECOND_PASS,
	};

	int fps;
	int i_Light;
	bool debugging;

	// stores the size of the map
	// indicates if user enabled lighting
	bool lightingEnabled;
	Light lights[LIGHT_COUNT];

	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Position viewSize;

	bool pause;

	Camera camera; // stationary

	Mesh* models[NUM_GEOMETRY];

	MS modelStack, viewStack, projectionStack;

	float elapseTime = 0;
	float bounceTime; // for key press inputs


	// applies material to geometry selected
	void applyMaterial(Mesh*);

	// renders gemotry
	void renderMesh(Mesh* model, bool enableLight = false);

	void initText();
	void renderText(Mesh* mesh, const std::string text, Color color);
	void renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

public:

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	void RenderFirstPass();
	void RenderSecondPass();
	void RenderScene();

	Camera& getCamera();

private:
	unsigned shadowShader;
	FrameBufferObject shadowFBO;

	Mtx44 lightProj;
	Mtx44 lightView;

	e_Passes e_Phases;

};

#endif