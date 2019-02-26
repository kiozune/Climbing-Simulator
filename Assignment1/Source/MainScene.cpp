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
	e_States = MAINMENU;
	//Initiallizing Variables for Text
	localR = 1.0f;
	localG = 1.0f;
	localSize = 2.0f;
	t_Pause = true;

	start_LocalR = 1.0f;
	start_LocalG = 1.0f;
	start_LocalSize = 2.0f;


	OnlineR = 1.0f;
	OnlineG = 0.0f;
	onlineSize = 3.5f;

	exitR = 1.0f;
	exitG = 1.0f;
	exitSize = 2.0f;

	rotateMap = 25.0f;

	onlineCheck = true;
    localCheck = false;
	exitCheck = false;
	startLocalCheck = false;

	//Ini textures into unsigned
	t_opaque = LoadTGA("Image//calibri.tga");
	t_alpha = LoadTGA("Image//calibriOpacity.tga");



	// clear screen and fill with white
	glClearColor(0.25, 0.25, 0.25, 0);
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
	m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");

	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOWMAP] = glGetUniformLocation(m_programID,"shadowMap");

	//Main Menu Shader
	menuShader = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	//Main Menu Uniforms
	m_parameters[U_MVP_MENU] = glGetUniformLocation(menuShader, "MVP");
	m_parameters[U_MODELVIEW_MENU] = glGetUniformLocation(menuShader, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE_MENU] = glGetUniformLocation(menuShader, "MV_inverse_transpose");

	m_parameters[U_MATERIAL_AMBIENT_MENU] = glGetUniformLocation(menuShader, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE_MENU] = glGetUniformLocation(menuShader, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR_MENU] = glGetUniformLocation(menuShader, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS_MENU] = glGetUniformLocation(menuShader, "material.kShininess");

	m_parameters[U_COLOR_TEXTURE_ENABLED_MENU] = glGetUniformLocation(menuShader, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE_MENU] = glGetUniformLocation(menuShader, "colorTexture");

	m_parameters[U_TEXT_ENABLED_MENU] = glGetUniformLocation(menuShader, "textEnabled");
	m_parameters[U_TEXT_COLOR_MENU] = glGetUniformLocation(menuShader, "textColor");


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

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_SHADOW_ENABLED] = glGetUniformLocation(shadowShader, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR] = glGetUniformLocation(shadowShader, "colorTexture[0]");
	for (int i = 0; i < LIGHT_COUNT; ++i)
		lights[i].getUniformLocation(m_programID);
	// Use our shader
	glUseProgram(m_programID);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

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

	models[SHADOW_QUAD] = MeshBuilder::GenerateQuad("Shadow_Quad", Color(1, 1, 1), 1.f);
	models[SHADOW_QUAD]->texArray[0] = shadowFBO.getTexture();

	models[TEST_OBJ] = MeshBuilder::GenerateOBJ("testLevel");
//	models[TEST_OBJ]->applyTexture("Image//skybox.tga");
	applyMaterial(models[TEST_OBJ]);
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

	glUseProgram(menuShader);


	initText();

	models[MAINMENU_QUAD] = MeshBuilder::GenerateScreen("Main_Menu", Color(1, 1, 1), 14.f);
	models[MAINMENU_QUAD]->applyTexture("Image//Background.tga");
	applyMaterial(models[MAINMENU_QUAD]);

	models[LOADING] = MeshBuilder::GenerateScreen("Loading", Color(1, 1, 1), 14.f);
	models[LOADING]->applyTexture("Image//controller.tga");
	applyMaterial(models[LOADING]);

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

	if (Application::IsKeyPressed('9'))
		dt /= 10;
	//Enter Inputs to change scene
	//When ur on Local
	if (localCheck == true)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			e_States = LOADINGSCREEN;
		}
	}
	//When ur on Online
	if (onlineCheck == true)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			e_States = LOADINGSCREEN;
		}
	}
	//WHen ur on Exit
	if (exitCheck == true)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			e_States = EXIT_GAME;
		}
	}
	//Transition from Start Local to Online
	//Default bouncetime for transitions set to 0.5
	if (Application::IsKeyPressed(VK_UP) && onlineCheck == false && localCheck == false && startLocalCheck == true && bounceTime < 0)
	{
	models[JOINONLINE_QUAD]->setTexture(t_opaque);
	bounceTime = 0.5f;
	OnlineG = 0;
	onlineSize = 3.5f;
	start_LocalSize = 2.0f;
	start_LocalG = 1;
	onlineCheck = true;
	localCheck = false;
	startLocalCheck = false;
	models[STARTLOCAL_QUAD]->setTexture(t_alpha);
	}
	//Transition from Start Local to Join Online
	if (Application::IsKeyPressed(VK_UP) && localCheck == false && exitCheck == false && startLocalCheck == true && bounceTime < 0)
	{
		models[STARTLOCAL_QUAD]->setTexture(t_opaque);
		bounceTime = 0.5f;
		start_LocalG = 0;
		localG = 1;
		start_LocalSize = 3.5f;
		localSize = 2.0f;
		localCheck = false;
		startLocalCheck = true;
		exitCheck = false;
		models[JOINLOCAL_QUAD]->setTexture(t_alpha);
	}
	//Transition from Join Local to start Local
	if (Application::IsKeyPressed(VK_UP) && localCheck == true && exitCheck == false && startLocalCheck == false && bounceTime < 0)
	{
		models[STARTLOCAL_QUAD]->setTexture(t_opaque);
		bounceTime = 0.5f;
		start_LocalG = 0;
		localG = 1;
		start_LocalSize = 3.5f;
		localSize = 2.0f;
		onlineCheck = false;
		startLocalCheck = true;
		localCheck = false;
		models[JOINLOCAL_QUAD]->setTexture(t_alpha);
	}
	//Transition from Exit to Join Local
	if (Application::IsKeyPressed(VK_UP) && localCheck == false && exitCheck == true && startLocalCheck == false && bounceTime < 0)
	{
	models[JOINLOCAL_QUAD]->setTexture(t_opaque);
	bounceTime = 0.5f;
	localG = 0;
	exitG = 1;
	localSize = 3.5f;
	exitSize = 2.0f;
	startLocalCheck = false;
	localCheck = true;
	exitCheck = false;
	models[EXIT]->setTexture(t_alpha);
	}
	//Transition from Online to Start Local
	if (Application::IsKeyPressed(VK_DOWN) && onlineCheck == true && startLocalCheck == false && localCheck == false && bounceTime < 0)
	{
	models[STARTLOCAL_QUAD]->setTexture(t_opaque);
	bounceTime = 0.5f;
	OnlineG = 1;
	onlineSize = 2.0f;
	start_LocalSize = 3.0f;
	start_LocalG = 0;
	onlineCheck = false;
	startLocalCheck = true;
	localCheck = false;
    models[JOINONLINE_QUAD]->setTexture(t_alpha);
	}
	//Transition from Start Local to Join Local
	if (Application::IsKeyPressed(VK_DOWN) && onlineCheck == false && localCheck == false && startLocalCheck == true && bounceTime < 0)
	{
		models[JOINLOCAL_QUAD]->setTexture(t_opaque);
		bounceTime = 0.5f;
		start_LocalG = 1;
		start_LocalSize = 2.0f;
		localSize = 3.0f;
		localG = 0;
		onlineCheck = false;
		localCheck = true;
		startLocalCheck = false;
		models[STARTLOCAL_QUAD]->setTexture(t_alpha);
	}
	//Transition from Local to Exit
	if (Application::IsKeyPressed(VK_DOWN) && onlineCheck == false && localCheck == true && exitCheck == false && bounceTime < 0)
	{
	models[EXIT]->setTexture(t_opaque);
	bounceTime = 0.5f;
	localG = 1;
	localSize = 2.0f;
	exitG = 0;
	exitSize = 3.5f;
	onlineCheck = false;
	localCheck = false;
	exitCheck = true;
	models[JOINLOCAL_QUAD]->setTexture(t_alpha);
	}

	rotateMap += 10 * dt;

	bounceTime -= dt;

}

void MainScene::Render()
{
		switch (e_States)
		{
		case MAINMENU:
		{
			renderMenu();
			break;
		}
		case GAMEMODE:
		{
			while (t_Pause == true)
			{
				Sleep(6000);
				t_Pause = false;
			}
			RenderFirstPass();
			RenderSecondPass();
			break;
		}
		case LOADINGSCREEN:
		{
			renderLoading();
			break;
		}
		default:
		{
			std::cout << "Are you sure theres a scene?" << std::endl;
			break;
		}
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
	{
		lightProj.SetToOrtho(-i_Light, i_Light, -i_Light, i_Light, -i_Light, i_Light);
	}
	else
	{
		lightProj.SetToPerspective(90.0f, 1.f, 0.1, 20);
	}

	lightView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	RenderScene();
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
	RenderScene();

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

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.2f, 0);
	modelStack.Scale(10, 10, 10);
	renderMesh(models[SHADOW_QUAD], false);
	modelStack.PopMatrix();

}
//Renders the world
void MainScene::RenderScene()
{
	modelStack.PushMatrix();
	modelStack.Scale(100, 100, 100);
	// renderMesh(models[SKY_BOX]);
	modelStack.PopMatrix();
	std::vector<Player*> localPlayers = players->getLocalPlayers();
	unsigned size = localPlayers.size();
	Vector3 frameSize = Application::getFrameSize();

	float rows = ceil((float)size / 2.0f);
	float columns = ceil((float)size / rows);

	float windowWidth = frameSize.x / columns;
	float windowHeight = frameSize.y / rows;

	for (int i = 0; i < size; ++i)
	{
		int x = i % 2;
		int y = i / 2;
		glViewport(windowWidth * x, windowHeight * y, windowWidth, windowHeight);
		renderForPlayer(localPlayers[i]);
	}

	if (size == 3)
	{
		std::vector<Player*> all = players->getPlayers();
		glViewport(windowWidth, windowHeight, windowWidth, windowHeight);
		renderTextOnScreen(models[TEXT], "SPECTATOR\nCAM", Color(1, 1, 1), 2, 1, 1);
		renderForPlayer(all[spectatingPlayer]);

		if (elapseTime - prevTime > 5)
		{
			spectatingPlayer++;
			spectatingPlayer = spectatingPlayer % all.size();
			prevTime = elapseTime;
		}
	}
	// renderMesh(models[SKY_BOX]);
	// renderMesh(models[AXES]);


	// renderTextOnScreen(models[TEXT], std::to_string(elapseTime), Color(1, 1, 1), 1, 2, 2);
}
//Renders Main Menu
void MainScene::renderMenu()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Using Shader without the shadow calculation

	glUseProgram(menuShader);
	//renders texts and main menu
	renderMenu2D(models[MAINMENU_QUAD], 11.0f, 14.0f, 11.0f, -0.8f, 1.25f);
	renderTextOnScreenMenu(models[JOINONLINE_QUAD], "JOIN ONLINE", Color(OnlineR,OnlineG, 1), onlineSize, 4, 20);
	renderTextOnScreenMenu(models[STARTLOCAL_QUAD], "START LOCAL", Color(start_LocalR, start_LocalG, 1), start_LocalSize, 4, 15);
	renderTextOnScreenMenu(models[JOINLOCAL_QUAD], "JOIN LOCAL", Color(localR, localG, 1), localSize, 4, 10);
	renderTextOnScreenMenu(models[EXIT], "EXIT", Color(exitR, exitG, 1), exitSize, 4, 5);

	glEnable(GL_DEPTH_TEST);
	modelStack.PushMatrix();
	modelStack.Translate(0.45f, -0.4f, -0.4f);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	modelStack.Rotate(rotateMap, 0, 1, 0);
	modelStack.Rotate(25, 1, 0, 0);
	renderMeshMenu(models[TEST_OBJ], false);
	modelStack.PopMatrix();
	glDisable(GL_DEPTH_TEST);
}
//Loading Screen Render
void MainScene::renderLoading()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Use Menu Shader
	glUseProgram(menuShader);
	//Render Loading Screen
	renderMenu2D(models[LOADING], 11.0f, 14.0f, 11.0f, -0.8f, 1.25f);
	e_States = GAMEMODE;
}

void MainScene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
//Returns enum to Application.cpp to check if exit is being called
int MainScene::getSceneEnum()
{
	return e_States;
}
