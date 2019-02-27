#include "AllScenes.h"

#include "ControllerManager.h"
#include "SceneManager.h"
#include "DataTransferManager.h"

#include "shader.hpp"
#include "LoadTGA.h"

void MenuScene::Init() 
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

	//models[TEST_OBJ] = MeshBuilder::GenerateOBJ("testLevel");
	//	models[TEST_OBJ]->applyTexture("Image//skybox.tga");
	//applyMaterial(models[TEST_OBJ]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(m_programID);

	//Ini textures into unsigned
	t_opaque = LoadTGA("Image//calibri.tga");
	t_alpha = LoadTGA("Image//calibriOpacity.tga");

	models[CREATE_LOBBY] = MeshBuilder::GenerateText("Start Local", 16, 16);
	models[CREATE_LOBBY]->applyTexture("Image//calibriOpacity.tga");
	models[JOIN_LOBBY] = MeshBuilder::GenerateText("Join_Online", 16, 16);
	models[JOIN_LOBBY]->applyTexture("Image//calibri.tga");
	models[CONTROLS] = MeshBuilder::GenerateText("Controls", 16, 16);
	models[CONTROLS]->applyTexture("Image//calibri.tga");
	models[EXIT_GAME] = MeshBuilder::GenerateText("EXIT", 16, 16);
	models[EXIT_GAME]->applyTexture("Image//calibriOpacity.tga");

	background = MeshBuilder::GenerateScreen("Main_Menu", Color(1, 1, 1), 14.f);
	background->applyTexture("Image//Background.tga");
}

void MenuScene::Update(double dt)
{
	ControllerManager* controller = ControllerManager::getInstance();

	elapseTime += (float)dt;
	if (elapseTime < bounceTime)
		return;

	Vector3 leftJS;
	if (controller->isPresent(0))
	{
		 controller->getInput(0);
		 leftJS = controller->getLeftJoystick();
	}


	if (Application::IsKeyPressed(VK_UP) || leftJS.y > 0.5)
	{
		current = (OPTION)(current - 1);
		if (current < 0) current = (OPTION)(OPTION::COUNT - 1);
		bounceTime = elapseTime + 0.2f;
	}

	if (Application::IsKeyPressed(VK_DOWN) || leftJS.y < -0.5)
	{
		current = (OPTION)((current + 1) % OPTION::COUNT);
		bounceTime = elapseTime + 0.2f;
	}

	models[CREATE_LOBBY]->setTexture(t_alpha);
	models[JOIN_LOBBY]->setTexture(t_alpha);
	models[CONTROLS]->setTexture(t_alpha);
	models[EXIT_GAME]->setTexture(t_alpha);

	models[current]->setTexture(t_opaque);

		
	if (Application::IsKeyPressed(VK_RETURN) || Application::IsControllerPressed(0, 1))
	{
		Scene* next;
		switch (current)
		{
		case CREATE_LOBBY:
		{
			LoadingScene* destination = new LoadingScene;
			destination->setDetails([](int& i) {
				MultiplayerManager* m_manager = MultiplayerManager::getInstance();
				m_manager->startSever();

				std::string ip;
				while (ip == "")
					ip = m_manager->getSever().getIp();

				std::cout << "Sever started on " << ip << std::endl;

				m_manager->connectTo(ip);
				m_manager->receive();
				m_manager->send();

				DataTransferManager* d_manager = DataTransferManager::getInstance();
				while ((i = d_manager->getClient().getStatus()) == 0) {}
			}, new MainScene, this);

			next = destination;
			break;
		}
		case JOIN_LOBBY:
			next = new JoinScene;
			break;
		case CONTROLS:
			ControllerManager::getInstance()->toggleOnlyController();
			bounceTime = elapseTime + 0.2f;
			return;
		default:
			next = nullptr;
			break;
		}
		SceneManager* s_manager = SceneManager::getInstance();
		s_manager->setNext(next);
		bounceTime = elapseTime + 0.2f;
	}
}


void MenuScene::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Using Shader without the shadow calculation

	//renders texts and main menu
	renderMenu2D(background, 11.0f, 14.0f, 11.0f, -0.8f, 1.25f);

	bool onlyController = ControllerManager::getInstance()->isOnlyController();
	Color selected = Color(1, 1, 1);

	renderTextOnScreen(models[CREATE_LOBBY], "CREATE LOBBY", selected, current == CREATE_LOBBY ? 4 : 3, 4, 20);
	renderTextOnScreen(models[JOIN_LOBBY], "JOIN LOBBY", selected, current == JOIN_LOBBY ? 4 : 3, 4, 15);
	renderTextOnScreen(models[CONTROLS], onlyController ? "CONTROLLER ONLY" : "CONTROLLER + KEYBOARD", selected, 3, 4, 10);
	renderTextOnScreen(models[EXIT_GAME], "EXIT", selected, current == EXIT_GAME ? 4 : 3, 4, 5);

	/*
	glEnable(GL_DEPTH_TEST);
	modelStack.PushMatrix();
	modelStack.Translate(0.45f, -0.4f, -0.4f);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	modelStack.Rotate(rotateMap, 0, 1, 0);
	modelStack.Rotate(25, 1, 0, 0);
	renderMeshMenu(models[TEST_OBJ], false);
	modelStack.PopMatrix();
	glDisable(GL_DEPTH_TEST);
*/
}

void MenuScene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

