#include "MainScene.h"

#include "GL\glew.h"
#include "Mtx44.h"
#include "Utility.h"

#include "shader.hpp"
#include "Application.h"

#include <iostream>

void MainScene::Init()
{
	srand(time(NULL));

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	projectionStack.LoadMatrix(projection);

	// clear screen and fill with white
	glClearColor(0, 0, 0, 0);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a VAO
	glGenVertexArrays(1, &m_vertexArrayID);
	// bind that VAO
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");

	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	for (int i = 0; i < LIGHT_COUNT; ++i)
		lights[i].getUniformLocation(m_programID);

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::SPOT;
	lights[0].position.Set(0, 300, 0);
	lights[0].setUniform();

	glUniform1i(m_parameters[U_NUMLIGHTS], LIGHT_COUNT);

	// initialisation of personal variables

	lightingEnabled = true;

	initText();

	models[LIGHT] = MeshBuilder::GenerateSphere("LIGHT", Color(1, 1, 1), 1, 36);

	models[SKY_BOX] = MeshBuilder::GenerateOBJ("skybox");
	models[SKY_BOX]->applyTexture("Image//skybox.tga");
	applyMaterial(models[SKY_BOX]);

	models[QUAD] = MeshBuilder::GenerateQuad("QUAD", Color(1, 1, 1), Position(1, 1, 1));
	applyMaterial(models[QUAD]);

	models[CUBE] = MeshBuilder::GenerateCube("CUBE", Color(1, 1, 1), 1, 1, 1);
	applyMaterial(models[CUBE]);

	FixedCamera camera;
	camera.Init(Vector3(0, 0, 0), 200, 100, 180);
	cameras.push_back(camera);

	//for (int i = 0; i < PLAYER_COUNT; ++i)
		//initPlayer(players[i], Vector3(0, 30, i * 10), transfer->getClient().getId());
	//playerManger->setMain(&(players[0]));

	//initPlayer(remotePlayers[0], Vector3(20, 30, 0));
	//playerManger->addRemotePlayer(&(remotePlayers[0]));

	players->fixMissingPlayers();
	initMap();
}

void MainScene::Update(double dt)
{

	elapseTime += dt;
	fps = 1 / dt;

	std::vector<Player*> localPlayers = players->getLocalPlayers();
	unsigned size = localPlayers.size();
	while (cameras.size() < size)
	{
		FixedCamera camera;
		camera.Init(Vector3(0, 0, 0), 200, 100, 180);
		cameras.push_back(camera);
	}

	// standard controls
	keyboardEvents(dt);

	manager->applyGravity(dt);

	for (Player* p : localPlayers)
		updatePlayer(p, dt);

	// std::string data = transfer->stringifyData(transfer->getPlayerData(players[0]));
	// remotePlayers[0].update(transfer->parseData(data));

	//camera.move(dt);
	Vector3 center = Vector3(0,0,0);

	if (size)
	{
		for (int i = 0; i < size; ++i)
		{
			center = players->getLocalPlayers()[i]->getBody()->getCenter();
			Vector3 target = Vector3(int(center.x / 5) * 5, int(center.y / 5) * 5, int(center.z / 5) * 5);
			cameras[i].setTarget(target);
		}
	}
	else
	{
		Vector3 target = Vector3(int(center.x / 5) * 5, int(center.y / 5) * 5, int(center.z / 5) * 5);
		cameras[0].setTarget(target);
	}

}

void MainScene::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<Player*> localPlayers = players->getLocalPlayers();
	unsigned size = localPlayers.size();
	float windowWidth = 3200 / (float)size;
	for (int i = 0; i < size; ++i)
	{
		glViewport(windowWidth * i, 0, windowWidth, 1800);
		//glViewport(windowWidth * i, 0, windowWidth * 2, 3200);
		renderForPlayer(localPlayers[i]);
	}

	renderTextOnScreen(models[TEXT], std::to_string(elapseTime), Color(1, 1, 1), 1, 2, 2);
}

void MainScene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}