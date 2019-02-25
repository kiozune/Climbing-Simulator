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
		MAINMENU_QUAD,
		JOINLOCAL_QUAD,
		JOINONLINE_QUAD,
		EXIT,
		SHADOW_QUAD,


		NUM_GEOMETRY,
	};

	enum e_Passes
	{
		FIRST_PASS,
		SECOND_PASS,
	};

	enum e_Scenes
	{
		MAINMENU,
		GAMEMODE,
		EXIT_GAME,
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
	unsigned menuShader;
	unsigned m_parameters[U_TOTAL];

	Position viewSize;

	bool pause;

	Camera camera; // stationary

	Mesh* models[NUM_GEOMETRY];

	MS modelStack, viewStack, projectionStack;

	float elapseTime = 0;
	float bounceTime; // for key press inputs
	

	//Color for RGB for texts
	//Join Local Text
	float localR;
	float localG;
	//Size for Local Text
	float localSize;

	//join Online Text
	float OnlineR;
	float OnlineG;
	//size for online Text
	float onlineSize;

	//Exit Text
	float exitR;
	float exitG;
	//size for Exit Text
	float exitSize;

	bool onlineCheck;
	bool localCheck;
	bool exitCheck;
	
	//Textures for Texts.
	unsigned t_opaque;
	unsigned t_alpha;


	// applies material to geometry selected
	void applyMaterial(Mesh*);

	// renders gemotry
	void renderMesh(Mesh* model, bool enableLight = false);
	void renderMenu2D(Mesh* model,float sizex,float sizey,float sizez,float x,float y, bool enableLight = false);
	void initText();
	void renderText(Mesh* mesh, const std::string text, Color color);
	void renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void renderTextOnScreenMenu(Mesh* mesh, std::string text, Color color, float size, float x, float y);
public:

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void renderMenu();
	virtual void Exit();
	void RenderFirstPass();
	void RenderSecondPass();
	void RenderScene();
	Camera& getCamera();
	int getSceneEnum();
private:
	unsigned shadowShader;
	FrameBufferObject shadowFBO;

	Mtx44 lightProj;
	Mtx44 lightView;

	e_Passes e_Phases;
	e_Scenes e_States;

};

#endif