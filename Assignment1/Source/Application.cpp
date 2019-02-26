
//Include GLEW
#include <GL/glew.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "Application.h"

#include "MainScene.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

bool Application::keys[GLFW_KEY_LAST] = {};

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	keys[key] = action == GLFW_PRESS;
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h); //update opengl the new window size
}

void joystick_callback(int joy, int event)
{
	if (event == GLFW_CONNECTED)
	{
		// The joystick was connected
		PlayerManager* manager = PlayerManager::getInstance();
		if (joy >= manager->getLocalPlayers().size())
		{
			Player* p = manager->createPlayer(joy);
			manager->addLocalPlayer(p);
		}
	}
	else if (event == GLFW_DISCONNECTED)
	{
		// The joystick was disconnected
		PlayerManager* manager = PlayerManager::getInstance();
		manager->removeLocalPlayer(joy);
	}
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::isControllerPresent(int joy)
{ 
	return glfwJoystickPresent(joy);
}

bool Application::IsControllerPressed(int joy, unsigned short key)
{
	if (!isControllerPresent(joy)) return false;
	//std::cout << key << " : " << keys[key] << std::endl;
	int count;
	const unsigned char* axes = glfwGetJoystickButtons(joy, &count);
	return axes[key];
}

const float* Application::getControllerAnalog(int joy)
{
	int count;
	const float* a = glfwGetJoystickAxes(joy, &count);

	return a;
}

Vector3 Application::GetMousePosition()
{
	double mouseX, mouseY;
	glfwGetCursorPos(m_window, &mouseX, &mouseY);
	return Vector3(mouseX, mouseY);
}

Vector3 Application::getFrameSize()
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	return Vector3(width, height);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);


	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(3200, 1800, "You let go, you lose", NULL, NULL);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glfwSetKeyCallback(m_window, key_callback);
	glfwSetJoystickCallback(joystick_callback);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

void Application::Run()
{
	//Main Loop
	Scene *scene = new MainScene;
	scene->Init();

	bool isMultiplayer = true;
	if (isMultiplayer)
	{
		MultiplayerManager* manager = MultiplayerManager::getInstance();
		manager->startSever();

		std::string ip;
		while (ip == "")
			ip = manager->getSever().getIp();

		std::cout << "Sever started on " << ip << std::endl;

		manager->connectTo(ip);
		manager->receive();
		manager->send();
	}


	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window))
	{
		scene->Update(m_timer.getElapsedTime());
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms

		if (IsKeyPressed(VK_ESCAPE))
		{
			MultiplayerManager* manager = MultiplayerManager::getInstance();
			manager->end();
			break;
		}
	} //Check if the ESC key had been pressed or if the window had been closed

	scene->Exit();
	delete scene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}