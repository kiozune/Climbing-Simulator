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

	float elapseTime = 0;
	float bounceTime; // for key press inputs

	// stores the size of the map
	// indicates if user enabled lighting
	bool lightingEnabled;
	Light lights[LIGHT_COUNT];

	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned menuShader;
	unsigned m_parameters[U_TOTAL];

	Position viewSize;

	std::vector<FixedCamera> cameras; // stationary

	Mesh* models[NUM_GEOMETRY];

	MS modelStack, viewStack, projectionStack;

	BlockGenerator* blockGen = BlockGenerator::GetInstance();
	SoundManager* sound = SoundManager::GetInstance();

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

	void keyboardEvents(double&);
	void joystickEvents(double&, int);

	void updatePlayer(Player*, double&);
	void renderForPlayer(Player*);

	void RenderFirstPass();
	void RenderSecondPass();
	void RenderGame();
	void RenderPause();
public:

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();



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
