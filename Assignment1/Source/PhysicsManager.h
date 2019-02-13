#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "Object.h"
#include "Spring.h"

#include <vector>

#define g 3.2

class PhysicsManager 
{
	
private:

	static PhysicsManager* instance;

	std::vector<Object*> objects;
	std::vector<Spring*> springs;

public:

	
	static PhysicsManager * getInstance();

	std::vector<Object*> getObjects();
	void addObject(Object*);
	void updateObjects();

	void addSpring(Spring*);
	void updateSprings();

	void applyGravity(float);
	void applyImpulse(Object*, Vector3, float);

};

#endif