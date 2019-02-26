#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Constants.h"

#include "Scene.h"
#include "..\FrameBufferObject.h"
#include "BlockGenerator.h"
#include "Sound.h"
#include "FixedCamera.h"

#include "PhysicsManager.h"
#include "ControllerManager.h"
#include "PlayerManager.h"

#include "Player.h"
#include "RemotePlayer.h"

#define LIGHT_COUNT 1

class MainScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		AXES,
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

		QUAD,
		CUBE,

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

	std::vector<FixedCamera> cameras; // stationary

	Mesh* models[NUM_GEOMETRY];

	MS modelStack, viewStack, projectionStack;

	BlockGenerator* blockGen = BlockGenerator::GetInstance();
	SoundManager* sound = SoundManager::GetInstance();

	float tempTime = 0;
	float elapseTime = 0;
	float bounceTime; // for key press inputs

	std::string ip = "127.0.0.1";

	//Pause timer for keybind
	bool t_Pause;

	PhysicsManager* manager = PhysicsManager::getInstance();
	ControllerManager* controller = ControllerManager::getInstance();
	PlayerManager* players = PlayerManager::getInstance();

	Vector3 prevMousePosition;

	int prevTime;
	int spectatingPlayer;

	// applies material to geometry selected
	void applyMaterial(Mesh*);
	void changeColour(Mesh*, Color);

	void renderMesh(Mesh* model, bool enableLight = false);
	void initText();
	void renderText(Mesh* mesh, const std::string text, Color color);
	void renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	void renderObject(Object*);
	void renderJoint(Joint*);
	void renderBoundingBox(BoundingBox);

	void initMap();

	void updatePlayer(Player*, double&);
	void renderForPlayer(Player*);

	void keyboardEvents(double&);
	void joystickEvents(double&, int);

public:

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void renderLoading();

	void RenderFirstPass();
	void RenderSecondPass();
	void RenderScene();

	void renderMeshMenu(Mesh* model, bool enableLight);

private:
	unsigned shadowShader;
	FrameBufferObject shadowFBO;

	Mtx44 lightProj;
	Mtx44 lightView;

	e_Passes e_Phases;
	//e_Scenes e_States;

};

#endif
