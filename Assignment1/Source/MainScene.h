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
		LOADING,
		STARTLOCAL_QUAD,
		SHADOW_QUAD,


		NUM_GEOMETRY,
	};

	enum e_Passes
	{
		FIRST_PASS,
		SECOND_PASS,
	};

	/*Enum for gameStates Add Extra scenes below Exit_Game
	  If Add Above EXIT_GAME modify scene->getSceneEnum() == 2 to + 1 in application.cpp*/
	enum e_Scenes
	{
		MAINMENU,
		GAMEMODE,
		EXIT_GAME,
		LOADINGSCREEN,
	};

	int fps;
	int i_Light;
	bool debugging;

	float rotateMap;

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
	
	//Start Local Text
	float start_LocalR;
	float start_LocalG;
	//Size for start Local Text
	float start_LocalSize;

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
	bool startLocalCheck;
	
	//Textures for Texts.
	unsigned t_opaque;
	unsigned t_alpha;
	unsigned t_Test;

	//Pause timer for keybind
	bool t_Pause;


	// applies material to geometry selected
	void applyMaterial(Mesh*);

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
	virtual void renderLoading();
	virtual void Exit();
	void RenderFirstPass();
	void RenderSecondPass();
	void RenderScene();
	void renderMeshMenu(Mesh* model, bool enableLight);
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