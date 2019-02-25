#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Constants.h"

#include "Scene.h"
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
		TEXT,
		LIGHT,
		SKY_BOX,

		QUAD,
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

	std::vector<FixedCamera> cameras; // stationary

	Mesh* models[NUM_GEOMETRY];

	MS modelStack, viewStack, projectionStack;

	float elapseTime = 0;
	float bounceTime; // for key press inputs

	PhysicsManager* manager = PhysicsManager::getInstance();
	ControllerManager* controller = ControllerManager::getInstance();
	DataTransferManager* transfer = DataTransferManager::getInstance();
	PlayerManager* players = PlayerManager::getInstance();

	Vector3 prevMousePosition;

	bool isXboxController = false;

	// applies material to geometry selected
	void applyMaterial(Mesh*);
	void changeColour(Mesh*, Color);

	// renders  gemotry
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

};

#endif