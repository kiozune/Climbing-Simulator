#include "Object.h"

Object::Object(float m, Bone* b) : mass(m), bone(b) {}

Bone* Object::getBone() { return this->bone; }

void Object::applyImpulse(Vector3 force, float dt)
{
	if (mass == 0) return;
	Vector3 a = Vector3(force.x / mass, force.y / mass, force.z / mass);
	this->bone->accelerate(a, dt);
}