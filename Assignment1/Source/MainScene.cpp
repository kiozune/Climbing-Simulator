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

	OnlineR = 1.0f;
	OnlineG = 0.0f;
	onlineSize = 3.5f;

	exitR = 1.0f;
	exitG = 1.0f;
	exitSize = 2.0f;

	onlineCheck = true;
    localCheck = false;
	exitCheck = false;

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
	lights[0].position.Set(3, 10, 0);
	lights[0].setUniform();

	glUniform1i(m_parameters[U_NUMLIGHTS], LIGHT_COUNT);


	//Camera Init
	camera.Init(Vector3(1,1,1), Vector3(0, 1, 0), 0, 0, 10, 10);

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	projectionStack.LoadMatrix(projection);


	// initialisation of personal variables

	lightingEnabled = true;

	models[LIGHT] = MeshBuilder::GenerateSphere("LIGHT", Color(1, 1, 1), 1, 36);

	models[SKY_BOX] = MeshBuilder::GenerateOBJ("skybox");
	models[SKY_BOX]->applyTexture("Image//skybox.tga");
	applyMaterial(models[SKY_BOX]);

	models[SHADOW_QUAD] = MeshBuilder::GenerateQuad("Shadow_Quad", Color(1, 1, 1), 1.f);
	models[SHADOW_QUAD]->texArray[0] = shadowFBO.getTexture();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(menuShader);

	
	initText();
	models[MAINMENU_QUAD] = MeshBuilder::GenerateScreen("Main_Menu", Color(1, 1, 1), 15);
	models[MAINMENU_QUAD]->applyTexture("Image//Background.tga");
	applyMaterial(models[MAINMENU_QUAD]);

}

void MainScene::Update(double dt)
{

	elapseTime += dt;
	fps = 1 / dt;

	// standard controls

	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if (Application::IsKeyPressed('7'))
		lightingEnabled = false;
	if (Application::IsKeyPressed('8'))
		lightingEnabled = true;

	if (Application::IsKeyPressed('9'))
		dt /= 10;
	if (localCheck == true)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			e_States = GAMEMODE;
		}
	}
	if (onlineCheck == true)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			e_States = GAMEMODE;
		}
	}
	if (exitCheck == true)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			e_States = EXIT_GAME;
		}
	}
	if (Application::IsKeyPressed(VK_UP) && onlineCheck == false && localCheck == true && exitCheck == false && bounceTime < 0)
	{
	models[JOINONLINE_QUAD]->setTexture(t_opaque);
	bounceTime = 0.5f;
	OnlineG = 0;
	onlineSize = 3.5f;
	localSize = 2.0f;
	localG = 1;
	onlineCheck = true;
	localCheck = false;
	exitCheck = false;
	models[JOINLOCAL_QUAD]->setTexture(t_alpha);
	}
	if (Application::IsKeyPressed(VK_UP) && localCheck == false && exitCheck == true && onlineCheck == false && bounceTime < 0)
	{
	models[JOINLOCAL_QUAD]->setTexture(t_opaque);
	bounceTime = 0.5f;
	localG = 0;
	exitG = 1;
	localSize = 3.5f;
	exitSize = 2.0f;
	onlineCheck = false;
	localCheck = true;
	exitCheck = false;
	models[EXIT]->setTexture(t_alpha);
	}
	if (Application::IsKeyPressed(VK_DOWN) && onlineCheck == true && localCheck == false && exitCheck == false && bounceTime < 0)
	{
	models[JOINLOCAL_QUAD]->setTexture(t_opaque);
	bounceTime = 0.5f;
	OnlineG = 1;
	onlineSize = 2.0f;
	localSize = 3.0f;
	localG = 0;
	onlineCheck = false;
	localCheck = true;
	exitCheck = false;
    models[JOINONLINE_QUAD]->setTexture(t_alpha);
	}
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


	bounceTime -= dt;
	camera.move(dt);
}

void MainScene::Render()
{
	if (e_States == MAINMENU)
	{
		renderMenu();
	}
	else if (e_States == GAMEMODE)
	{
		RenderFirstPass();
		RenderSecondPass();
	}
}

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

	// get reference to camera based on state
	Vector3 target = camera.getTarget();
	Vector3 up = camera.getUp();

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
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
			modelStack.Rotate(90, 1, 0, 0);
			renderMesh(models[LIGHT]);
		}
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.2, 0);
	modelStack.Scale(10, 10, 10);
	renderMesh(models[SHADOW_QUAD], false);
	modelStack.PopMatrix();

}

void MainScene::RenderScene()
{
	modelStack.PushMatrix();
	modelStack.Scale(100, 100, 100);
	renderMesh(models[SKY_BOX]);
	modelStack.PopMatrix();
}

void MainScene::renderMenu()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(menuShader);
	// get reference to camera based on state
	renderMenu2D(models[MAINMENU_QUAD], 11.0f, 12.0f, 9.0f, 0, 1);
	renderTextOnScreenMenu(models[JOINONLINE_QUAD], "JOIN ONLINE", Color(OnlineR,OnlineG, 0), onlineSize, 4, 20);
	renderTextOnScreenMenu(models[JOINLOCAL_QUAD], "JOIN LOCAL", Color(localR, localG, 0), localSize, 4, 15);
	renderTextOnScreenMenu(models[EXIT], "EXIT", Color(exitR, exitG, 0), exitSize, 4, 10);
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

int MainScene::getSceneEnum()
{
	return e_States;
}