#include "MainMenu.h"

#include "GL\glew.h"
#include "Mtx44.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "shader.hpp"
#include "Application.h"

#include <iostream>

void MainMenu::Init()
{
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

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

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
	camera.Init(Vector3(), Vector3(0, 1, 0), 0, 0, 10, 10);

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	projectionStack.LoadMatrix(projection);


	// initialisation of personal variables

	lightingEnabled = true;

	initText();
	models[LIGHT] = MeshBuilder::GenerateSphere("LIGHT", Color(1, 1, 1), 1, 36);
	models[MAIN_SCREEN] = MeshBuilder::GenerateQuad("Screen", Color(1, 1, 1), Position(1, 1, 1));

}

void MainMenu::Update(double dt)
{

	elapseTime += dt;
	fps = 1 / dt;
	if (Application::IsKeyPressed('O'))
	{
		
	}
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

	camera.move(dt);
}

void MainMenu::Render()
{
	glViewport(0, 0, Application::getWidth(), Application::getHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 cam_perspective;
	cam_perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(cam_perspective);

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
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(10, 10, 10);
	renderMesh(models[MAIN_SCREEN], false);
	modelStack.PopMatrix();

}

void MainMenu::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

Camera& MainMenu::getCamera()
{
	return this->camera;
}