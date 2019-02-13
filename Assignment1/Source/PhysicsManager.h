#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "Object.h"

#include <vector>

class PhysicsManager 
{
	
private:

	static PhysicsManager* instance;

	std::vector<Object*> objects;

public:

	
	static PhysicsManager * getInstance();

	std::vector<Object*> getObjects();
	void addObject(Object*);
	void updateObjects();

	void applyGravity(float);
	void applyImpulse(Object*, Vector3, float);
};

#endif