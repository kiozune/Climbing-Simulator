#ifndef MAINSCENE_H
#define MAINSCENE_H

#define LSPEED 10.0f

#include "Scene.h"
#include "../FrameBufferObject.h"

#define LIGHT_COUNT 1

class MainScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		TEXT,
		LIGHT,
		SKY_BOX,
		TEST_OBJ,
		SHADOW_QUAD,

		NUM_GEOMETRY,
	};

	enum e_Passes
	{
		FIRST_PASS,
		SECOND_PASS,
	};

	int fps;
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
	unsigned int frameBuffer;
	unsigned int frameBufferShader;

	MS modelStack, viewStack, projectionStack;

	float elapseTime = 0;
	float bounceTime; // for key press inputs


	// applies material to geometry selected
	void applyMaterial(Mesh*);

	// renders geometry
	void renderMesh(Mesh* model, bool enableLight = false);
	
	//For Shadows
	FrameBufferObject shadows;

	void initText();
	void renderText(Mesh* mesh, const std::string text, Color color);
	void renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void renderFirstPass(); // First Pass Rendering from light's POV
	void renderSecondPass(); // Second Pass Rendering from Camera's POV
	void renderScene();

	e_Passes e_phase;
	Mtx44 lightProjection;
	Mtx44 lightView;
	int i_Lightview;
public:
	static const int i_Max = 2;


	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	Camera& getCamera();
};

#endif