#include "ControllerManager.h"

#include "Application.h"

ControllerManager* ControllerManager::instance = nullptr;

ControllerManager* ControllerManager::getInstance()
{
	if (instance == nullptr) instance = new ControllerManager;
	return instance;
}

bool ControllerManager::isPresent() { return Application::isControllerPresent(this->joy); }
bool ControllerManager::isPresent(int j) { return Application::isControllerPresent(j); }

void ControllerManager::getInput(int joy)
{
	this->joy = joy;
	if (this->isPresent())
	{
		this->analog = Application::getControllerAnalog(joy);
	}
}

Vector3 ControllerManager::getLeftJoystick() 
{ 
	if (this->isPresent())
	{
		const float x = this->analog[0];
		const float y = -this->analog[1];
		return Vector3(x, y, 0);
	}
	
	return Vector3();
}

Vector3 ControllerManager::getRightJoystick() 
{ 
	if (this->isPresent())
	{
		const float x = -int(this->analog[2] * 10) / 10.0;
		const float y = int(this->analog[5] * 10) / 10.0;
		return Vector3(x, y, 0); 
	}

	return Vector3();
}

float ControllerManager::getLT() 
{ 
	if (this->isPresent())
		return this->analog[3] + 1; 
	
	return 0;
}

float ControllerManager::getRT() 
{ 
	if (this->isPresent())
		return this->analog[4] + 1; 
	
	return 0;
}