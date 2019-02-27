#ifndef MAINSCENE_H
#define MAINSCENE_H

#define LSPEED 10.0f

#include "Scene.h"
#include "..\FrameBufferObject.h"
#include <string>
#define LIGHT_COUNT 1

class MainScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		TEXT,
		LIGHT,
		SKY_BOX,
		TEST_OBJ,
		LOBBY_QUAD,
		MAINMENU_QUAD,
		ONLINELOBBY_QUAD,
		LOCALLOBY_QUAD,
		JOINLOCAL_QUAD,
		STARTLOCAL_QUAD,
		JOINONLINE_QUAD,
		STARTONLINE_QUAD,
		EXIT,
		LOADING,
		BACK_QUAD,
		SHADOW_QUAD,
		Metaphor_QUAD,
		LOSE_QUAD,
		WIN_QUAD,


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
		LOBBY,
		ONLINELOBBY,
		LOCALLOBBY,
		LOSESCREEN,
		WINSCREEN,
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
	

	char inputField_Array[11];

	bool pause;

	Camera camera; // stationary

	Mesh* models[NUM_GEOMETRY];

	MS modelStack, viewStack, projectionStack;

	float elapseTime = 0;
	float bounceTime; // for key press inputs
	
	//Color for RGB for texts
	//Lobby Text
	float lobbyR;
	float lobbyG;
	float lobbySize;


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
	//Join Local Text
	float join_LocalR;
	float join_LocalG;
	//Join Local Size
	float join_LocalSize;

	//join Online Text
	float start_OnlineR;
	float start_OnlineG;
	//size for online Text
	float start_onlineSize;

	//Online Text
	float OnlineR;
	float OnlineG;
	//size for online Text
	float onlineSize;

	//Join Online Text
	float join_OnlineR;
	float join_OnlineG;
	//size for Join Online Text
	float join_OnlineSize;

	//Create Online Text
	float create_OnlineR;
	float create_OnlineG;
	//Size for Create online Text
	float create_OnlineSize;

	//Exit Text
	float exitR;
	float exitG;
	//size for Exit Text
	float exitSize;

	//Back Text
	float backR;
	float backG;
	//size for Back Text
	float backSize;
	//Boolean checks for transitions
	bool onlineCheck;
	bool joinOnlineCheck;
	bool createOnlineCheck;
	bool localCheck;
	bool exitCheck;
	bool startLocalCheck;
	bool joinLocalCheck;
	bool lobbyCheck;
	bool backCheck;
	
	//Textures for Texts.
	unsigned t_opaque;
	unsigned t_alpha;
	unsigned t_Test;

	//Pause timer for keybind
	bool t_Pause;

	//count for Random Metaphors in lose Screen
	int i_rLose;

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
	void renderMenu();
	void renderLoading();
	void renderLobby();
	void renderOnline();
	void renderLocal();
	void renderLoseScreen();
	void renderWinScreen();
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