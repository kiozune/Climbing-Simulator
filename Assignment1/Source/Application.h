#ifndef APPLICATION_H
#define APPLICATION_H

#include "MultiplayerManager.h"

#include "timer.h"
#include "Vector3.h"

#include <GLFW/glfw3.h>

class Application {

public:

	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool isControllerPresent(int joy);
	static bool IsControllerPressed(int joy, unsigned short key);
	static const float* getControllerAnalog(int joy);
	static Vector3 GetMousePosition();
	static Vector3 getFrameSize();

private:


	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod);

	static bool keys[GLFW_KEY_LAST];

	//Declare a window object
	StopWatch m_timer;
};

#endif