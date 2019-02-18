#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

#include <GLFW/glfw3.h>

class Application {

public:

	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static int getWindowWidth();
	static int getWindowHeight();

private:

	//Declare a window object
	StopWatch m_timer;
	//Window Screen Values
	const static int window_Width = 1600;
	const static int window_Height = 900;
};

#endif