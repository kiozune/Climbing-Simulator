#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "Object.h"
#include "Spring.h"
#include "CollisionDetails.h"

#include <vector>

#define grav 5
#define fr 0.5

class PhysicsManager 
{
	
private:

	static PhysicsManager* instance;

	std::vector<Object*> others;
	std::vector<Object*> environment;
	std::vector<Object*> objects;

	std::vector<Spring*> springs;
	std::vector<Spring**> externalSprings;

public:	
	
	static PhysicsManager * getInstance();

	std::vector<Object*> getObjects();
	void addObject(Object*);
	void updateObjects();

	std::vector<Object*> getEnvironment();
	void addToEnvironment(Object*);

	CollisionDetails getEnviromentalCollision(Object*);
	CollisionDetails getCollisionDetails(Object*, std::vector<Object*>);
	void resolveCollisions();


	void addSpring(Spring*);
	void addExternalSpring(Spring**);
	void updateSprings();

	void applyGravity(float);
	void applyImpulse(Object*, Vector3, float);

	void destroy();

};

#endif