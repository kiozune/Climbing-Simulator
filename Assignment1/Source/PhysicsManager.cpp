#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::instance = nullptr;

PhysicsManager* PhysicsManager::getInstance() 
{
	if (instance == nullptr) instance = new PhysicsManager;
	return instance;
}

std::vector<Object> PhysicsManager::getObjects() { return this->objects; }

void PhysicsManager::addObject(Object obj) { this->objects.push_back(obj); }

void PhysicsManager::applyGravity(float dt) 
{  
	for (Object& obj : objects) 
	{
		obj.getBone()->accelerate(Vector3(0, -9.8, 0), dt);
	}
}