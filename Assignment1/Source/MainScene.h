#ifndef MAINSCENE_H
#define MAINSCENE_H

#define LSPEED 10.0f

#include "Scene.h"
#include "FixedCamera.h"

#include "PhysicsManager.h"
#include "ControllerManager.h"

#include "Player.h"

#define LIGHT_COUNT 1
#define PLAYER_COUNT 2

class MainScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		TEXT,
		LIGHT,
		SKY_BOX,

		CUBE,

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

	FixedCamera camera; // stationary

	Mesh* models[NUM_GEOMETRY];

	MS modelStack, viewStack, projectionStack;

	float elapseTime = 0;
	float bounceTime; // for key press inputs

	PhysicsManager* manager = PhysicsManager::getInstance();
	ControllerManager* controller = ControllerManager::getInstance();

	Vector3 prevMousePosition;

	bool isXboxController = false;

	Player players[PLAYER_COUNT];

	// applies material to geometry selected
	void applyMaterial(Mesh*);

	// renders gemotry
	void renderMesh(Mesh* model, bool enableLight = false);

	void initText();
	void renderText(Mesh* mesh, const std::string text, Color color);
	void renderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	void renderObject(Object*);
	void renderJoint(Joint*);
	void renderBoundingBox(BoundingBox);

	void initPlayer(Player&, Vector3);
	void initMap();

	void keyboardEvents(double&);
	void joystickEvents(double&);

public:

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	Camera& getCamera();
};

#endif