#include "MainScene.h"

#include "GL\glew.h"
#include "Mtx44.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "shader.hpp"
#include "Application.h"

#include <iostream>

void MainScene::Init()
{
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

	//frame Buffer Shader
	shadowShader = LoadShaders("Shader//depthTexture.vertexshader", "Shader//depthTexture.fs");

	m_parameters[U_LIGHT_DEPTH_MVP_FIRSTPASS] = glGetUniformLocation(shadowShader, "lightDepthMVP");
	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOWMAP] = glGetUniformLocation(m_programID,"shadowMap");

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

	m_parameters[U_SHADOW_ENABLED] = glGetUniformLocation(shadowShader, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR] = glGetUniformLocation(shadowShader, "colorTexture[0]");

	for (int i = 0; i < LIGHT_COUNT; ++i)
		lights[i].getUniformLocation(m_programID);

	glUseProgram(m_programID);

	lights[0].type = Light::SPOT;
	lights[0].position.Set(0, 300, 0);
	lights[0].setUniform();

	glUniform1i(m_parameters[U_NUMLIGHTS], LIGHT_COUNT);

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	projectionStack.LoadMatrix(projection);

	// initialisation of personal variables

	lightingEnabled = true;

	models[LIGHT] = MeshBuilder::GenerateSphere("LIGHT", Color(1, 1, 1), 1, 36);
	models[AXES] = MeshBuilder::GenerateAxes("axes", Position(10, 10, 10));

	/*models[SKY_BOX] = MeshBuilder::GenerateOBJ("skybox");
	models[SKY_BOX]->applyTexture("Image//skybox.tga");
	applyMaterial(models[SKY_BOX]);*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	models[QUAD] = MeshBuilder::GenerateQuad("QUAD", Color(1, 1, 1), Position(1, 1, 1));
	applyMaterial(models[QUAD]);

	models[CUBE] = MeshBuilder::GenerateCube("CUBE", Color(1, 1, 1), 1, 1, 1);
	applyMaterial(models[CUBE]);


	FixedCamera camera;
	camera.Init(Vector3(0, 0, 0), 200, 100, 180);
	cameras.push_back(camera);

	players->addLocalPlayer(players->createPlayer(0));

	players->fixMissingPlayers();
	initMap();
	sound->playSound("bgm");

	initText();
	initMenu();
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

	if (ColResult.collided)
		printf("triggered");

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
	if (isPaused)
	{
		RenderPause();
	}
	else
	{
		RenderFirstPass();
		RenderSecondPass();
	}
}

//Rendering from the Lights point of view
void MainScene::RenderFirstPass()
{
	e_Phases = FIRST_PASS;
	shadowFBO.BindFrameBuffer();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowShader);

	if (lights[0].type == Light::DIRECTIONAL)
		lightProj.SetToOrtho(-i_Light, i_Light, -i_Light, i_Light, -i_Light, i_Light);
	else
		lightProj.SetToPerspective(90.0f, 1.f, 0.1, 20);

	lightView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	
	RenderGame();
}

//Rendering from the camera's Point of view
void MainScene::RenderSecondPass()
{
	e_Phases = SECOND_PASS;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::getWidth(), Application::getHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);

	shadowFBO.BindTexture(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MVP], 8);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 cam_perspective;
	cam_perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(cam_perspective);

	RenderGame();

	Vector3 position = cameras[0].getPosition();
	Vector3 target = cameras[0].getTarget();
	Vector3 up = cameras[0].getUp();

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
			Vector3 lightDirection_cameraspace = vs * lightDir;
			glUniform3fv(light.parameters[Light::L_POSITION], 1, &lightDirection_cameraspace.x);
		}
		else if (light.type == Light::SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light.position;
			glUniform3fv(light.parameters[Light::L_POSITION], 1, &lightPosition_cameraspace.x);
			Mtx44 vs = viewStack.Top();
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
			modelStack.Rotate(90, 1, 0, 0);
			renderMesh(models[LIGHT]);
		}
		modelStack.PopMatrix();
	}
}

void MainScene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
