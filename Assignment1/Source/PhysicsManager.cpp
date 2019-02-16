#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::instance = nullptr;

PhysicsManager* PhysicsManager::getInstance() 
{
	if (instance == nullptr) instance = new PhysicsManager;
	return instance;
}

std::vector<Object*> PhysicsManager::getObjects() { return this->objects; }

void PhysicsManager::addObject(Object* obj) 
{ 
	this->objects.push_back(obj); 
	this->others.push_back(obj);
}

void PhysicsManager::updateObjects(float dt) 
{
	for (Object* obj : others)
	{
		for (Object* env : environment)
		{
			if (obj->getBoundingBox().didCollideWith(env->getBoundingBox()))
			{
				//obj->applyImpulse(-obj->getMomentum() * obj->getMass() * fr, 1);
			}
		}
	}

	for (Object* obj : this->objects)
		obj->constraint();
}

std::vector<Object*> PhysicsManager::getEnvironment() { return this->environment; }

void PhysicsManager::addToEnvironment(Object* obj)
{
	this->environment.push_back(obj);
	this->objects.push_back(obj);
}

void PhysicsManager::addSpring(Spring* spr) { this->springs.push_back(spr); }
void PhysicsManager::addExternalSpring(Spring** spr) { this->externalSprings.push_back(spr); }

void PhysicsManager::updateSprings()
{
	for (Spring* spr : this->springs)
		spr->constraint();

	for (Spring** spr : this->externalSprings)
		(*spr)->constraint();
}

void PhysicsManager::applyGravity(float dt) 
{  
	for (Object* obj : this->objects)
		if (obj->isAffectByGravity())
			obj->accelerate(Vector3(0, -g, 0), dt);
}

void PhysicsManager::applyImpulse(Object* obj, Vector3 force, float dt)
{
	obj->applyImpulse(force, dt);
	this->updateObjects(dt);
}