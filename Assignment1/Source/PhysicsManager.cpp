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

void PhysicsManager::updateObjects() 
{
	for (Object* obj : this->objects)
		obj->constraint();
}

std::vector<Object*> PhysicsManager::getEnvironment() { return this->environment; }

void PhysicsManager::addToEnvironment(Object* obj)
{
	this->environment.push_back(obj);
	this->objects.push_back(obj);
}

CollisionDetails PhysicsManager::getEnviromentalCollision(Object* obj)
{
	CollisionDetails details;
	for (Object* env : environment)
	{
		details.result = obj->getBoundingBox().getCollisionResultWith(env->getBoundingBox());
		details.object = env;
		if (details.result.collided)
			break;
	}
	return details;
}

CollisionDetails PhysicsManager::getCollisionDetails(Object* main, std::vector<Object*> objects)
{
	CollisionDetails details;
	for (Object* obj : objects)
	{
		details.result = main->getBoundingBox().getCollisionResultWith(obj->getBoundingBox());
		details.object = obj;
		if (details.result.collided)
			break;
	}
	return details;
}

void PhysicsManager::resolveCollisions()
{
	for (Object* obj : others)
	{
		obj->getStart()->resetResolve();
		obj->getEnd()->resetResolve();
	}
	CollisionResult result;
	for (Object* obj : others)
	{
		if (obj->isClippingEnabled()) continue;
		for (Object* env : environment)
		{
			if (env->isClippingEnabled()) continue;
			result = obj->getBoundingBox().getCollisionResultWith(env->getBoundingBox());
			if (result.collided)
			{
				Vector3 displacement = result.displacement;
				obj->getStart()->displace(displacement);
				obj->getEnd()->displace(displacement);
			}
		}
	}
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
			obj->accelerate(Vector3(0, -grav, 0), dt);
}

void PhysicsManager::applyImpulse(Object* obj, Vector3 force, float dt)
{
	obj->applyImpulse(force, dt);
	this->updateObjects();
}

void PhysicsManager::destroy()
{
	for (Object* obj : objects)
		delete obj;

	objects.clear();
	others.clear();
	environment.clear();

	for (Spring* spr : springs)
		delete spr;

	springs.clear();

	for (Spring** spr : externalSprings)
		delete *spr;

	externalSprings.clear();
}