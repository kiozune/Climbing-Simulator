#ifndef APPLICATION_H
#define APPLICATION_H

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
	static Vector3 GetMousePosition();

private:

	//Declare a window object
	StopWatch m_timer;
};

#endif