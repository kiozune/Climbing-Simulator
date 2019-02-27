#include "AllScenes.h"
#include "SceneManager.h"

#include "MultiplayerManager.h"
#include "DataTransferManager.h"
#include "shader.hpp"

void JoinScene::Init()
{
	ignore = true;

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(m_programID);

	text = MeshBuilder::GenerateText("TEXT", 16, 16);
	text->applyTexture("Image//calibri.tga");
}

void JoinScene::Update(double dt)
{
	elapseTime += (float)dt;

	char possible[11] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	if (elapseTime < bounceTime)
		return;

	for (char& c : possible)
	{
		if (Application::IsKeyPressed(c))
		{
			ip += c;
			bounceTime = elapseTime + 0.2f;
			break;
		}
	}

	if (Application::IsKeyPressed(VK_OEM_PERIOD))
	{
		ip += '.';
		bounceTime = elapseTime + 0.2f;
	}

	if (Application::IsKeyPressed(VK_BACK))
	{
		if (ip.size()) ip.pop_back();
		bounceTime = elapseTime + 0.2f;
	}

	if (Application::IsKeyPressed(VK_RETURN))
	{
		if (!ignore)
		{
			SceneManager* s_manager = SceneManager::getInstance();
			LoadingScene* destination = new LoadingScene;
			s_manager->setNext(destination);

			MultiplayerManager* m_manager = MultiplayerManager::getInstance();
			m_manager->connectTo(ip);
			
			destination->setDetails([](int& i) {
				MultiplayerManager* m_manager = MultiplayerManager::getInstance();
				m_manager->receive();
				m_manager->send();

				DataTransferManager* manager = DataTransferManager::getInstance();
				while ((i = manager->getClient().getStatus()) == 0) {}
			}, new MainScene, new MenuScene);
		}
		else
		{
			ignore = false;
		}
		bounceTime = elapseTime + 0.2f;
	}

	if (Application::IsKeyPressed(VK_ESCAPE))
	{
		SceneManager* s_manager = SceneManager::getInstance();
		s_manager->setNext(new MenuScene);
		bounceTime = elapseTime + 0.2f;
	}
}

void JoinScene::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderTextOnScreen(text, "ENTER IP\n" + ip, Color(1, 1, 1), 3, 2, 2);
}

void JoinScene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
