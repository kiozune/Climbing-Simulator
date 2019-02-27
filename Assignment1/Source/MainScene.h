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
		Metaphor_QUAD,
		LOSE_QUAD,
		WIN_QUAD,

		QUAD,
		CUBE,

		NUM_GEOMETRY,
	};

	enum e_Passes
	{
		FIRST_PASS,
		SECOND_PASS,
	};

	enum OPTION
	{
		RESUME_GAME,
		EXIT_GAME,
		COUNT,
	};
	OPTION current;

	int fps;
	int i_Light;
	bool debugging;

	bool isPaused;

	float elapseTime = 0;
	float bounceTime; // for key press inputs


	// stores the size of the map
	// indicates if user enabled lighting
	bool lightingEnabled;
	Light lights[LIGHT_COUNT];

	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Position viewSize;


	char inputField_Array[11];

	std::vector<FixedCamera> cameras; // stationary

	Mesh* models[NUM_GEOMETRY];
	Mesh* menuModels[COUNT];

	unsigned t_opaque;
	unsigned t_alpha;

	MS modelStack, viewStack, projectionStack;

	BlockGenerator* blockGen = BlockGenerator::GetInstance();
	SoundManager* sound = SoundManager::GetInstance();
	CollisionResult ColResult;
	Object* finishingPlatform;

	PhysicsManager* manager = PhysicsManager::getInstance();
	ControllerManager* controller = ControllerManager::getInstance();
	PlayerManager* players = PlayerManager::getInstance();

	Vector3 prevMousePosition;


	float prevTime;
	int spectatingPlayer;

	//count for Random Metaphors in lose Screen
	int i_rLose;


	// applies material to geometry selected
	void applyMaterial(Mesh*);
	void changeColour(Mesh*, Color);

	void renderMesh(Mesh* model, bool enableLight = false);
	void renderMenu2D(Mesh* model, float sizex, float sizey, float sizez, float x, float y);
	void initText();
	void renderText(Mesh* mesh, const std::string text, Color color);
	void renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void renderTextOnScreenMenu(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	void renderObject(Object*);
	void renderJoint(Joint*);
	void renderBoundingBox(BoundingBox);

	void initMap();

	void keyboardEvents(double&);
	void joystickEvents(double&, int);

	void updatePlayer(Player*, double&);
	void renderForPlayer(Player*);

	void initMenu();

	void RenderFirstPass();
	void RenderSecondPass();
	void RenderGame();
	void RenderPause();
	void renderLoseScreen();
	void renderWinScreen();

public:

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	unsigned shadowShader;
	FrameBufferObject shadowFBO;

	Mtx44 lightProj;
	Mtx44 lightView;

	e_Passes e_Phases;
	//e_Scenes e_States;

};

#endif
