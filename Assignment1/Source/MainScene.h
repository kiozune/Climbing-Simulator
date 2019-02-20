#ifndef MAINSCENE_H
#define MAINSCENE_H

#define LSPEED 10.0f

#include "Scene.h"
#include "BlockGenerator.h"

#define LIGHT_COUNT 1

class MainScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		AXES,
		TEXT,
		LIGHT,
		SKY_BOX,

		NUM_GEOMETRY,
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

	MS modelStack, viewStack, projectionStack;

	BlockGenerator* blockGen = BlockGenerator::GetInstance();
	
	float elapseTime = 0;
	float bounceTime; // for key press inputs


	// applies material to geometry selected
	void applyMaterial(Mesh*);

	// renders gemotry
	void renderMesh(Mesh* model, bool enableLight = false);
	
	// render blocks
	void renderBlocks();

	void initText();
	void renderText(Mesh* mesh, const std::string text, Color color);
	void renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

public:

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	Camera& getCamera();
};

#endif