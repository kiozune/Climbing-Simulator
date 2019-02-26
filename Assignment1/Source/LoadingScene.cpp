#include "LoadingScene.h"

#include "SceneManager.h"
#include "shader.hpp"

void LoadingScene::Init()
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

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	projectionStack.LoadMatrix(projection);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(m_programID);

	text = MeshBuilder::GenerateText("TEXT", 16, 16);
	text->applyTexture("Image//calibri.tga");
}

void LoadingScene::Update(double dt)
{
	elapseTime += dt;
	if (state > 0)
	{
		SceneManager* manager = SceneManager::getInstance();
		manager->setNext(destination);
	}
	else if (state < 0)
	{
		SceneManager* manager = SceneManager::getInstance();
		manager->setNext(source);
	}
}

void LoadingScene::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderTextOnScreen(text, "LOADING...", Color(1, 1, 1), 3, 2, 2);
}

void LoadingScene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void LoadingScene::setDetails(void(*a)(int&), Scene* d, Scene* s)
{
	this->state = 0;

	this->action = a;
	this->action(this->state);

	this->destination = d;
	this->source = s;
}