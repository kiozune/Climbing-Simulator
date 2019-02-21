#include "ControllerManager.h"

#include "Application.h"

ControllerManager* ControllerManager::instance = nullptr;

ControllerManager* ControllerManager::getInstance()
{
	if (instance == nullptr) instance = new ControllerManager;
	return instance;
}

void ControllerManager::getInput(int joy)
{
	if (Application::isControllerPresent(joy))
		this->analog = Application::getControllerAnalog(joy);
}

Vector3 ControllerManager::getLeftJoystick() 
{ 
	return Vector3(this->analog[0], -this->analog[1], 0); 
}

Vector3 ControllerManager::getRightJoystick() 
{ 
	const float x = -int(this->analog[2] * 10) / 10.0;
	const float y = int(this->analog[5] * 10) / 10.0;
	return Vector3(x, y, 0); 
}

float ControllerManager::getLT() { return this->analog[3] + 1; }
float ControllerManager::getRT() { return this->analog[4] + 1; }