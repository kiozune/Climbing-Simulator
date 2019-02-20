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

	models[CUBE] = MeshBuilder::GenerateCube("CUBE", Color(1, 1, 1), 1, 1, 1);
	applyMaterial(models[CUBE]);

	camera.Init(Vector3(0, 0, 0), 200, 100, 180);

	for (int i = 0; i < PLAYER_COUNT; ++i) 
		initPlayer(players[i], Vector3(0, 0, i * 10));

	initMap();
}

void MainScene::Update(double dt)
{

	elapseTime += dt;
	fps = 1 / dt;

	// standard controls
	keyboardEvents(dt);



	manager->applyGravity(dt);

	for (int i = 0; i < PLAYER_COUNT; ++i)
		updatePlayer(i, dt);

	manager->resolveCollisions();



	//camera.move(dt);
	Vector3 center = players[0].getBody()->getCenter();
	Vector3 target = Vector3(int(center.x / 5) * 5, int(center.y / 5) * 5, int(center.z / 5) * 5);
	camera.setTarget(target);
}

void MainScene::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get reference to camera based on state
	Vector3 position = camera.getPosition();
	Vector3 target = camera.getTarget();
	Vector3 up = camera.getUp();

	viewStack.LoadIdentity(); 
	viewStack.LookAt(
		position.x, position.y, position.z,
		target.x, target.y, target.z,
		up.x, up.y, up.z
	);

	modelStack.LoadIdentity();

	for (Light& light : lights) {
		if (light.type == Light::DIRECTIONAL)
		{
			Vector3 lightDir(light.position.x, light.position.y, light.position.z);
			Mtx44 vs = viewStack.Top();
			vs.a[12] = vs.a[13] = vs.a[14] = 0;
			Vector3 lightDirection_cameraspace = vs * lightDir;
			glUniform3fv(light.parameters[Light::L_POSITION], 1, &lightDirection_cameraspace.x);
		}
		else if (light.type == Light::SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light.position;
			glUniform3fv(light.parameters[Light::L_POSITION], 1, &lightPosition_cameraspace.x);
			Mtx44 vs = viewStack.Top();
			vs.a[12] = vs.a[13] = vs.a[14] = 0;
			Vector3 spotDirection_cameraspace = vs * light.spotDirection;
			glUniform3fv(light.parameters[Light::L_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * light.position;
			glUniform3fv(light.parameters[Light::L_POSITION], 1, &lightPosition_cameraspace.x);
		}

		// render light
		modelStack.PushMatrix();
		{
			modelStack.Translate(light.position.x, light.position.y, light.position.z);
			renderMesh(models[LIGHT]);
		}
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	//renderMesh(models[SKY_BOX]);
	modelStack.PopMatrix();

	for (Object* obj : manager->getObjects()) 
	{
		renderObject(obj);
		//renderBoundingBox(obj->getBoundingBox());
	}

	float yaw = camera.getYaw();
	float pitch = camera.getPitch();

	for (Player& p : players)
	{
		Vector3 left  = p.getLeftHand()->getCenter();
		Vector3 right = p.getRightHand()->getCenter();
		modelStack.PushMatrix();
		{
			modelStack.Translate(left.x, left.y + 5, left.z);

			modelStack.Rotate(270 - yaw, 0, 1, 0);
			modelStack.Rotate(pitch, 1, 0, 0);

			modelStack.Scale(3, 3, 3);

			Color color = p.isGrabbingLeft() ? Color(.9, .9, 0) : Color(1, 1, 1);
			renderText(models[TEXT], "LT", color);
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		{
			modelStack.Translate(right.x, right.y + 5, right.z);

			modelStack.Rotate(270 - yaw, 0, 1, 0);
			modelStack.Rotate(pitch, 1, 0, 0);

			modelStack.Scale(3, 3, 3);

			Color color = p.isGrabbingRight() ? Color(0, .9, .9) : Color(1, 1, 1);
			renderText(models[TEXT], "RT", color);
		}
		modelStack.PopMatrix();
	}
}

void MainScene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

Camera& MainScene::getCamera() 
{
	return this->camera;
}