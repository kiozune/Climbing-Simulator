#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include "Vector3.h"

class ControllerManager
{

private:

	static ControllerManager* instance;

	const float* analog;

public:

	static ControllerManager * getInstance();

	void getInput(int);

	Vector3 getLeftJoystick();
	Vector3 getRightJoystick();

	float getLT();
	float getRT();

};

#endif